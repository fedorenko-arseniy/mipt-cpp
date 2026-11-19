# HW2 — Rental fleet

A C++17 console program for managing rental equipment. It tracks daily energy use, lists vehicles suitable for beginners, and lets the operator select one item for display.

## Build and run

From the `hw2` directory, run

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o fleet
./fleet
```

Enter one command per line. Each item needs a unique ID and a single-word name. Daily energy must be finite and nonnegative. Beginner suitability scores range from 1 to 10.

| Command | Action |
| --- | --- |
| `add scooter ID NAME ENERGY SCORE` | Add an electric scooter. |
| `add bike ID NAME SCORE` | Add a bicycle. |
| `add ebike ID NAME ENERGY SCORE` | Add an electric bicycle. |
| `add eskate ID NAME ENERGY SCORE` | Add an electric skateboard. |
| `add helmet ID NAME` | Add a helmet. |
| `add dock ID NAME ENERGY` | Add a docking station. |
| `list` | Print IDs and names sorted by ID. |
| `novice` | List vehicles with a suitability score of at least 6. |
| `energy` | Print total daily energy use. |
| `stats` | Print the item count and total daily energy use. |
| `diversity` | Print the percentage of item pairs that have different types. |
| `focus ID` | Select an item for display. |
| `show_focus` | Print the selected item, or `expired` if the display is empty. |
| `remove ID` | Remove an item from the fleet. |
| `quit` | Exit. |

## Class diagram and ownership

```text
InventoryItem
└── NamedItem
    ├── Helmet
    ├── Dock          (also implements EnergyConsumer)
    └── Transport
        ├── Bike
        ├── Scooter   (also implements EnergyConsumer)
        ├── EBike     (also implements EnergyConsumer)
        └── ESkate    (also implements EnergyConsumer)
```

A `unique_ptr` owns each new item during inspection. Inspection rejects vehicles whose names contain `broken`, ignoring case. If inspection throws, the candidate is destroyed automatically. On success, the fleet takes ownership through a `shared_ptr` stored in its registry. The display holds a `weak_ptr` to the selected item, so removing that item makes the selection expire.

## Example

```text
add scooter S1 Swift 1.0 7
add scooter S2 Comet 1.0 5
add eskate K1 Board 0.8 8
stats
diversity
focus K1
remove K1
show_focus
quit
```

Output

```text
3 2.8
66.7%
expired
```

Two of the three possible pairs contain a scooter and a skateboard. The remaining pair contains two scooters. The diversity calculation includes every inventory type and returns `0.0%` for fewer than two items.

## AI assistance

I used AI to help format this README and its text-based class diagram, translate the documentation from Russian into English, and generate test cases.
