# GridNexus — Smart Decentralized Power Grid Simulation

A C++ simulation of a decentralized smart power grid, built for an Object-Oriented Programming course. It models three interconnected electrical districts — producers, consumers, and prosumers — that generate, consume, and redistribute energy across a 7-day simulation.

## What It Does

Instead of one centralized power source, GridNexus models multiple independent **grids**, each containing its own mix of energy nodes:

- **Producers** — Solar and Wind generators, affected by weather (sunlight/wind conditions)
- **Consumers** — Houses and Hospitals, each with different usage patterns and criticality
- **Prosumers** — Smart Houses that both generate (via solar) and consume energy, using battery storage to manage their own surplus/deficit

Each grid calculates its own energy balance every simulation day. A `GridNetwork` layer then matches grids with a surplus against grids with a deficit and transfers energy between them, so the system balances itself without central control.

## Key Features

- Decentralized architecture with 3 independent grids
- Prosumer model with battery storage (Smart Houses)
- Weather and usage mutations applied each day for realism
- Demand forecasting via a moving-average `PredictionEngine`, with error tracking
- Inter-grid energy transfers logged as `EnergyTransaction` records
- Full simulation log (`simulation_log.txt`) and transfer summary (`summary.txt`) written to file each run

## Object-Oriented Design

```
Node (abstract base)
├── Producer (virtual base)
│   ├── Solar
│   └── Wind
└── Consumer (virtual base)
    ├── House
    ├── Hospital
    └── SmartHouse (multiple inheritance: House + Solar)
```

Concepts used:
- **Encapsulation** — private members with controlled access
- **Inheritance & virtual inheritance** — `SmartHouse` inherits from both `House` and `Solar`; virtual inheritance avoids the diamond problem on the shared `Node` base
- **Polymorphism** — virtual methods for weather/usage mutation, resolved at runtime
- **Composition** — `Grid` contains `Node`s, `GridNetwork` contains `Grid`s
- **Operator overloading** — `<<` for readable output, `+=` for weather/usage mutations

## Simulation Workflow

Each simulated day:
1. Predict next day's demand from history (from day 2 onward)
2. Apply random weather/usage mutations to nodes
3. Simulate generation and consumption across all nodes
4. Compute each grid's energy balance
5. Redistribute surplus energy to deficit grids via `GridNetwork`
6. Log the day's activity and running summary

## Build & Run

This was built and tested on Windows (it uses the Windows API for colored console output).

```bash
g++ main.cpp -o gridnexus
./gridnexus
```

Running it produces/updates `simulation_log.txt` (detailed day-by-day log) and `summary.txt` (transaction summary) in the working directory.

## Project Files

| File | Purpose |
|---|---|
| `main.cpp` | Full simulation source |
| `simulation_log.txt` | Sample output log from a run |
| `summary.txt` | Sample transaction summary from a run |
| `GridNexusFinalReport.docx` | Final project report |
| `OOP Project Proposal(GridNexus).docx` | Original project proposal |

## Limitations & Future Work

- Weather/usage values are randomly generated rather than driven by real data
- No transmission losses are modeled between grids
- Demand forecasting uses a simple moving average, not a real predictive model
- Possible extensions: real weather API integration, transmission loss simulation, pricing/trading between grids, and visualizing transaction logs

## Course

Object-Oriented Programming — FAST NUCES, Karachi
