#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <sstream>
#include <windows.h>
#include <algorithm> // use min()
#include <cmath> // absolute value 
#include <cstdlib> // RAND()
#include <ctime> // to seed the RAND()

using namespace std;

// Global weather variables
int globalSunlight = 100;
int globalWind = 100;

// Color functions
void ColorPrint(char str[], int color);
void redPrint(char str[]);
void greenPrint(char str[]);
void bluePrint(char str[]);

void ColorPrint(char str[], int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    printf("%s", str);
    SetConsoleTextAttribute(hConsole, 7);
}

void redPrint(char str[]) { ColorPrint(str, 12); }
void greenPrint(char str[]) { ColorPrint(str, 10); }
void bluePrint(char str[]) { ColorPrint(str, 9); }



class Logger {
public:
    static void log(const string& message) {
        ofstream file("simulation_log.txt", ios::app);
        if (file.is_open()) {
            file << message << endl;
        } else {
            cerr << "Fail to open simulation_log.txt" << endl;
        }
    }
};

class Node {
    // Polymorphic mutation hooks
    public:
        virtual void mutateWeather(int sunlightChange, int windChange) {}
        virtual void mutateUsage(int change) {}
protected:
    int id;
    string name;
    double netEnergy;

public:
    Node(int id, string name) : id(id), name(name), netEnergy(0.0) {}
    virtual ~Node() = default;

    virtual void simulate() = 0;
    virtual void update() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getNetEnergy() const { return netEnergy; }
    void setNetEnergy(double energy) { netEnergy = energy; }
    void addEnergy(double amount) { netEnergy += amount; }
    void deductEnergy(double amount) { netEnergy -= amount; }

    friend ostream& operator<<(ostream& os, const Node& node) {
        os << "Node [" << node.id << "] " << node.name << " - Net Energy: " << node.netEnergy;
        return os;
    }
};

class EnergyTransaction {
private:
    Node* sender;
    Node* receiver;
    double amount;
    int transactionID;
    static int idCounter;

public:
    static void resetCounter() {
        idCounter = 0;
    }

    EnergyTransaction(Node* sender, Node* receiver, double amt)
        : sender(sender), receiver(receiver), amount(amt) {
            transactionID = ++idCounter;
        }

    void execute() {
        if (sender && receiver) {
            sender->deductEnergy(amount);
            receiver->addEnergy(amount);
        }
    }

    void display() const {
        cout << toString() << endl;
    }

    int getTransactionID() const { return transactionID; }

    string toString() const {
        ostringstream ss;
        ss << "Txn [" << transactionID << "]: Node " 
           << (sender ? to_string(sender->getId()) : "None") << " -> Node " 
           << (receiver ? to_string(receiver->getId()) : "None") << " | Amount: " << amount;
        return ss.str();
    }

    friend ostream& operator<<(ostream& os, const EnergyTransaction& trans) {
        os << trans.toString();
        return os;
    }
};
int EnergyTransaction::idCounter = 0;

class Producer : virtual public Node {
protected:
    double generationCapacity;
public:
    Producer(int id, string name, double capacity)
        : Node(id, name), generationCapacity(capacity) {}

    virtual void generateEnergy() {
        netEnergy += generationCapacity;
    }

    virtual void simulate() override {
        generateEnergy();
    }
    
    double getGenerationCapacity() const { return generationCapacity; }
};

class Solar : virtual public Producer {
public:
    void mutateWeather(int sunlightChange, int windChange) override {
        *this += sunlightChange;
    }

    Solar& operator+=(int change) {
        generationCapacity += change; 
        if (generationCapacity < 0) generationCapacity = 0;
        return *this;
    }

protected:
    double sunlightFactor; 
public:
    Solar(int id, string name, double capacity, double sunlight = 1.0)
        : Node(id, name), Producer(id, name, capacity), sunlightFactor(sunlight) {}

    void generateEnergy() override {
        netEnergy += (generationCapacity * sunlightFactor);
    }
};

class Wind : virtual public Producer {
public:
    void mutateWeather(int sunlightChange, int windChange) override {
        *this += windChange;
    }

    Wind& operator+=(int change) {
        generationCapacity += change; 
        if (generationCapacity < 0) generationCapacity = 0;
        return *this;
    }

protected:
    double windSpeed; 
public:
    Wind(int id, string name, double capacity, double speed = 10.0)
        : Node(id, name), Producer(id, name, capacity), windSpeed(speed) {}

    void generateEnergy() override {
        netEnergy += (generationCapacity * (windSpeed / 10.0));
    }
};

class Consumer : virtual public Node {
public:
    void mutateUsage(int change) override {
        *this += change;
    }

    Consumer& operator+=(int change) {
        consumptionRate += change; 
        if (consumptionRate < 0) consumptionRate = 0;
        return *this;
    }

protected:
    double consumptionRate;
public:
    Consumer(int id, string name, double rate)
        : Node(id, name), consumptionRate(rate) {}

    virtual void consumeEnergy() {
        netEnergy -= consumptionRate;
    }

    virtual void simulate() override {
        consumeEnergy();
    }
    
    double getConsumptionRate() const { return consumptionRate; }
};

class House : virtual public Consumer {
protected:
    string usageLevel;
public:
    House(int id, string name, double rate, string usage = "normal")
        : Node(id, name), Consumer(id, name, rate), usageLevel(usage) {}

    void consumeEnergy() override {
        double multiplier = 1.0;
        if (usageLevel == "high") multiplier = 1.5;
        else if (usageLevel == "low") multiplier = 0.5;
        netEnergy -= (consumptionRate * multiplier);
    }
};

class Hospital : virtual public Consumer {
protected:
    double criticalLoad;
public:
    Hospital(int id, string name, double rate, double cLoad)
        : Node(id, name), Consumer(id, name, rate), criticalLoad(cLoad) {}

    void consumeEnergy() override {
        double total = max(consumptionRate, criticalLoad);
        netEnergy -= total;
    }
};

class SmartHouse : public House, public Solar {
protected:
    double storedEnergy;
public:
    SmartHouse(int id, string name, double consumeRate, double produceCapacity, double storage = 100.0)
        : Node(id, name), Producer(id, name, produceCapacity), Consumer(id, name, consumeRate), 
          House(id, name, consumeRate, "normal"), Solar(id, name, produceCapacity, 1.0),
          storedEnergy(storage) {}

    void generateEnergy() override {
        Solar::generateEnergy();
    }

    void consumeEnergy() override {
        House::consumeEnergy();
    }

    void manageEnergy() {
        if (netEnergy > 0) {
            storedEnergy += netEnergy; // Store excess
            netEnergy = 0;
        } else if (netEnergy < 0) {
            double deficit = -netEnergy;
            if (storedEnergy >= deficit) {
                storedEnergy -= deficit; // Use battery
                netEnergy = 0;
            } else {
                netEnergy += storedEnergy; // Empty battery to offset some deficit
                storedEnergy = 0;
            }
        }
    }

    void simulate() override {
        generateEnergy();
        consumeEnergy();
        manageEnergy();
    }

    double getStoredEnergy() const { return storedEnergy; } // Optional nice feature to log
};

class GridNetwork;

class Grid {
private:
    int gridId;
    string location;
    double totalGeneration;
    double totalConsumption;
    double gridEnergyBalance;
    vector<Node*> nodes;

    friend class GridNetwork;

public:
    Grid(int id, string loc) : gridId(id), location(loc), totalGeneration(0), totalConsumption(0), gridEnergyBalance(0) {}

    ~Grid() {
        for (auto node : nodes) {
            delete node;
        }
    }

    void addNode(Node* node) {
        nodes.push_back(node);
    }

    void computeBalance() {
        totalGeneration = 0;
        totalConsumption = 0;
        for (auto& node : nodes) {
            double n = node->getNetEnergy();
            if (n > 0) totalGeneration += n;
            else totalConsumption -= n; 
        }
        gridEnergyBalance = totalGeneration - totalConsumption;
    }

    void simulateGrid() {
        for (auto& node : nodes) {
            node->setNetEnergy(0.0);
            node->simulate();
        }
        computeBalance();
    }

    void importEnergy(double amount) {
        gridEnergyBalance += amount;
    }

    void exportEnergy(double amount) {
        gridEnergyBalance -= amount;
    }

    double getSurplus() const { return gridEnergyBalance; }
    double getGridConsumption() const { return totalConsumption; }
    double getTotalGeneration() const { return totalGeneration; } // Helper for new logging logic
    int getId() const { return gridId; }
    string getLocation() const { return location; }
    vector<Node*>& getNodes() { return nodes; }
};

class GridNetwork {
private:
    vector<Grid*> grids;

public:
    void addGrid(Grid* grid) {
        grids.push_back(grid);
    }

    void transferEnergy(Grid* source, Grid* destination, double amount, int day) {
        source->exportEnergy(amount);
        destination->importEnergy(amount);

        Node* sNode = source->getNodes().empty() ? nullptr : source->getNodes().front();
        Node* dNode = destination->getNodes().empty() ? nullptr : destination->getNodes().front();
        
        EnergyTransaction txn(sNode, dNode, amount);
        txn.execute(); // Balances the nodes personally
        
        // Log transaction messaging as requested
        ostringstream ss;
        ss << "Transaction: Transferred " << amount << " units from Grid " 
           << source->getId() << " to Grid " << destination->getId() << ".";
        string msgWithNewline = ss.str() + "\n";
        bluePrint((char*)msgWithNewline.c_str());
        Logger::log(ss.str());

        // Write to summary.txt
        ofstream summaryFile("summary.txt", ios::app);
        if (summaryFile.is_open()) {
            summaryFile << "D" << day << "T" << txn.getTransactionID() << ": From "
                        << source->getLocation() << "(" << source->getId() << ") To "
                        << destination->getLocation() << "(" << destination->getId() << ") Units: " << amount << "\n";
        }
    }

    void balanceEnergy(int day) {
        vector<Grid*> surplusGrids;
        vector<Grid*> deficitGrids;

        for (auto grid : grids) {
            if (grid->getSurplus() > 0) surplusGrids.push_back(grid);
            else if (grid->getSurplus() < 0) deficitGrids.push_back(grid);
        }

        for (auto sGrid : surplusGrids) {
            for (auto dGrid : deficitGrids) {
                if (sGrid->getSurplus() <= 0) break;
                if (dGrid->getSurplus() >= 0) continue;

                double amountToTransfer = min(sGrid->getSurplus(), -dGrid->getSurplus());
                transferEnergy(sGrid, dGrid, amountToTransfer, day);
            }
        }
    }

    void simulateGridsOnly() {
        for (auto grid : grids) {
            grid->simulateGrid();
        }
    }
};

class PredictionEngine {
private:
    vector<double> history;
    double lastPrediction;

public:
    PredictionEngine() : lastPrediction(0.0) {}

    void addData(double demand) {
        history.push_back(demand);
    }

    double predictDemand() {
        if (history.empty()) return 0.0;
        double sum = 0.0;
        for (double d : history) sum += d;
        lastPrediction = sum / history.size();
        return lastPrediction;
    }

    double calculateError(double actual) {
        return abs(lastPrediction - actual);
    }
   };

// Global log helper wrap function
void sysLog(const string& text) {
    cout << text << "\n";
    Logger::log(text);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    sysLog("========== 7-DAY SIMULATION STARTED ==========");

    // Create 3 grids
    Grid* grid1 = new Grid(1, "North District");
    Grid* grid2 = new Grid(2, "South District");
    Grid* grid3 = new Grid(3, "Central District");

    grid1->addNode(new Solar(101, "Solar Farm 1", 500.0, 1.0));
    grid1->addNode(new Wind(102, "Wind Turbine 1", 300.0, 10.0));
    grid1->addNode(new House(103, "House 1", 50.0, "high"));
    grid1->addNode(new Hospital(104, "Hospital North", 400.0, 600.0));
    grid1->addNode(new SmartHouse(105, "Smart House North", 80.0, 60.0, 40.0));

    grid2->addNode(new Solar(201, "Solar Farm 2", 600.0, 0.9));
    grid2->addNode(new Wind(202, "Wind Turbine 2", 350.0, 12.0));
    grid2->addNode(new House(203, "House 2", 60.0, "low"));
    grid2->addNode(new Hospital(204, "City Hospital", 600.0, 800.0));
    grid2->addNode(new SmartHouse(205, "Smart House South", 100.0, 80.0, 50.0));

    grid3->addNode(new Solar(301, "Solar Farm 3", 450.0, 1.1));
    grid3->addNode(new Wind(302, "Wind Turbine 3", 320.0, 9.0));
    grid3->addNode(new House(303, "House 3", 55.0, "normal"));
    grid3->addNode(new Hospital(304, "Central Hospital", 500.0, 900.0));
    grid3->addNode(new SmartHouse(305, "Smart House Central", 90.0, 70.0, 60.0));

    GridNetwork network;
    network.addGrid(grid1);
    network.addGrid(grid2);
    network.addGrid(grid3);

    PredictionEngine predictor1;
    PredictionEngine predictor2;
    PredictionEngine predictor3;

    vector<Grid*> grids = {grid1, grid2, grid3};
    vector<PredictionEngine*> predictors = {&predictor1, &predictor2, &predictor3};

    for (int day = 1; day <= 7; ++day) {
        EnergyTransaction::resetCounter();
        string dayStr = "\n==============================\n         DAY " + to_string(day) + "\n==============================\n";
        bluePrint((char*)dayStr.c_str());
        Logger::log("\n==============================");
        Logger::log("         DAY " + to_string(day));
        Logger::log("==============================");

        // Weather Phase Output
        int sunlightChange = (rand() % 41) - 10;
        int windChange = (rand() % 41) - 10;
        globalSunlight += sunlightChange;
        globalWind += windChange;

        ostringstream wss;
        wss << "Change in Weather: Sunlight changed by " << sunlightChange 
            << ", Wind changed by " << windChange << ".";
        sysLog(wss.str());

        // Apply Weather and Usage Random Mutations safely into class scopes
        for (auto grid : grids) {
            for (Node* node : grid->getNodes()) {
                node->mutateWeather(sunlightChange, windChange);
                int usageChange = (rand() % 41) - 10;
                node->mutateUsage(usageChange);
            }
        }

        // Processing & Generation Output
        network.simulateGridsOnly();
        
        sysLog("\n--- Actual Generation vs Demand ---");
        for (int i = 0; i < 3; ++i) {
            ostringstream gss;
            gss << "Grid " << (i+1) << " Actual Generation: " << grids[i]->getTotalGeneration()
                << " | Actual Demand: " << grids[i]->getGridConsumption();
            sysLog(gss.str());
        }

        // Initiating Transfers Outputs 
        network.balanceEnergy(day); 
        
        bool allMet = true;
        for (int i = 0; i < 3; ++i) {
            if (grids[i]->getSurplus() < 0) {
                allMet = false;
                break;
            }
        }
        
        if (allMet) {
            greenPrint((char*)"ALL GRIDS CAN MEET THE DEMAND AFTER ENERGY TRANSACTION\n");
            Logger::log("ALL GRIDS CAN MEET THE DEMAND AFTER ENERGY TRANSACTION");
        } else {
            redPrint((char*)"ALL GRIDS CAN NOT MEET THE DEMAND AFTER ENERGY TRANSACTION\n");
            Logger::log("ALL GRIDS CAN NOT MEET THE DEMAND AFTER ENERGY TRANSACTION");
        }

        for (int i = 0; i < 3; ++i) {
            predictors[i]->addData(grids[i]->getGridConsumption());
        }

        // Prediction Phase Output (predicting for next day)
        sysLog("\n--- Predicted Demand for Next Day ---");
        for (int i = 0; i < 3; ++i) {
            ostringstream pss;
            pss << "Grid " << (i+1) << " Predicted Demand: " << predictors[i]->predictDemand();
            sysLog(pss.str());
        }
    }

    sysLog("\n========== 7-DAY SIMULATION ENDED ==========\n");

    delete grid1;
    delete grid2;
    delete grid3;

    return 0;
}
