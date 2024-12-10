/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  SternGerlachSimulator.cpp
 */

#include "SternGerlachSimulator.h"
#include <random>
#include <cmath>

const double PI = 3.14159265358979323846;

// Remove the duplicate State struct definition
// struct State {
//     double theta, phi;
// };

// Now use the global State struct defined in the header

struct SGDevice {
    char type;
    int row;
    int col;
    
    // Add operator== for QList operations
    bool operator==(const SGDevice& other) const {
        return type == other.type && row == other.row && col == other.col;
    }
};

SternGerlachSimulator::SternGerlachSimulator(QObject *parent) 
    : QObject(parent) {
    results = SimResults{0, 0, 0, 0, 0.0, 0.0, 0.0};
}

bool SternGerlachSimulator::findPattern(const QVariantList& grid, QVector<char>& sgDirections, 
                                        QVector<bool>& blockSpinUp) {
    const int WIDTH = 16; // width and height of the entire grid
    const int HEIGHT = 16;

    // Clear existing vectors
    sgDirections.clear();
    blockSpinUp.clear();

    // Define patterns with their respective dimensions
    struct Pattern {
        const QStringList patternData;
        int height;
        int width;
    };

    // Define the original patterns (3x13)
    Pattern pattern1 = {
        {
            ". . . . . . . . Y . - . o",
            ". . . . Z . . . . . | . .",
            "X . . . . . . . . . . . ."
        },
        3, 13
    };

    Pattern pattern2 = {
        {
            ". . . . . . . . Y . | . .",
            ". . . . Z . . . . . - . o",
            "X . . . . . . . . . . . ."
        },
        3, 13
    };

    // Define the new pattern (2x9)
    Pattern pattern3 = {
        {
            ". . . . X . - . o",
            "Z . . . . . | . ."
        },
        2, 9
    };

    // Store all patterns in a list
    QVector<Pattern> patterns = {pattern1, pattern2, pattern3};

    // Convert incoming grid to 2D array
    QVector<QVector<QString>> currentGrid(HEIGHT, QVector<QString>(WIDTH));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            currentGrid[i][j] = grid[i * WIDTH + j].toString();
        }
    }

    // Try each pattern
    for (const auto& pat : patterns) {
        int patternHeight = pat.height;
        int patternWidth = pat.width;

        // Convert pattern data to 2D array
        QVector<QVector<QString>> patternGrid;
        for (const QString& rowStr : pat.patternData) {
            QStringList rowList = rowStr.split(' ', Qt::SkipEmptyParts);
            patternGrid.append(rowList.toVector());
        }

        // Try each possible position in the grid
        for (int i = 0; i <= HEIGHT - patternHeight; i++) {
            for (int j = 0; j <= WIDTH - patternWidth; j++) {
                bool matches = true;

                // Check if pattern matches at current position
                for (int pi = 0; pi < patternHeight && matches; pi++) {
                    for (int pj = 0; pj < patternWidth && matches; pj++) {
                        if (currentGrid[i + pi][j + pj] != patternGrid[pi][pj]) {
                            matches = false;
                        }
                    }
                }

                if (matches) {
                    // Pattern found - set directions and spin states
                    if (pat.patternData == pattern3.patternData) {
                        // Handling the new pattern (2 SG devices)
                        sgDirections = {'X', 'Z'};  // Order: bottom to top
                        blockSpinUp = {true};       // Only one blocking decision needed
                                                    // between the two SG devices
                    } else {
                        // Existing patterns (3 SG devices)
                        sgDirections = {'X', 'Z', 'Y'};  // Order: bottom to top
                        blockSpinUp = {true, true, false}; // Corresponding spin states

                        if (pat.patternData == pattern2.patternData) {
                            // Adjust spin states if necessary for pattern2
                            blockSpinUp[2] = true; // Example adjustment
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

State SternGerlachSimulator::generateInitialState(const QString& initialState) {
    State state{0.0, 0.0};
    
    if (initialState == "+Z") {
        state.theta = 0;
        state.phi = 0;
    } else if (initialState == "-Z") {
        state.theta = PI;
        state.phi = 0;
    } else if (initialState == "+X") {
        state.theta = PI/2;
        state.phi = 0;
    } else if (initialState == "-X") {
        state.theta = PI/2;
        state.phi = PI;
    } else if (initialState == "+Y") {
        state.theta = PI/2;
        state.phi = PI/2;
    } else if (initialState == "-Y") {
        state.theta = PI/2;
        state.phi = 3*PI/2;
    } else if (initialState.startsWith("custom")) {
        QStringList parts = initialState.split(',');
        if (parts.size() == 3) {
            state.theta = parts[1].toDouble();
            state.phi = parts[2].toDouble();
        }
    }
    return state;
}

State SternGerlachSimulator::SG_Measurement(const State& input, char direction) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    
    double probability;
    if (direction == 'Z') {
        probability = std::cos(input.theta/2) * std::cos(input.theta/2);
    } else if (direction == 'X') {
        probability = 0.5 * (1 + std::sin(input.theta) * std::cos(input.phi));
    } else if (direction == 'Y') {
        probability = 0.5 * (1 + std::sin(input.theta) * std::sin(input.phi));
    }

    State output;
    if (dist(gen) < probability) {
        output.theta = 0;    // Spin up
    } else {
        output.theta = PI;   // Spin down
    }
    output.phi = 0;
    return output;
}

State SternGerlachSimulator::setState(char direction, bool spinUp) {
    State output;
    if ((direction == 'Z' || direction == 'z') && spinUp) {
        output.theta = 0;
        output.phi = 0;
    } else if ((direction == 'Z' || direction == 'z') && !spinUp) {
        output.theta = PI;
        output.phi = 0;
    } else if ((direction == 'X' || direction == 'x') && spinUp) {
        output.theta = PI / 2;
        output.phi = 0;
    } else if ((direction == 'X' || direction == 'x') && !spinUp) {
        output.theta = PI / 2;
        output.phi = PI;
    } else if ((direction == 'Y' || direction == 'y') && spinUp) {
        output.theta = PI / 2;
        output.phi = PI / 2;
    } else if ((direction == 'Y' || direction == 'y') && !spinUp) {
        output.theta = PI / 2;
        output.phi = 3 * PI / 2;
    }
    return output;
}

void SternGerlachSimulator::runSimulation(const QString& initialState,
                                          const QVector<char>& sgDirections,
                                          const QVector<bool>& blockSpinUp,
                                          int particleCount) {
    State state = generateInitialState(initialState);

    if (sgDirections.size() == 2) {
        int countUpFinal = 0, countDownFinal = 0;

        // Run each particle through both SGs in sequence
        for (int i = 0; i < particleCount; i++) {
            // First SG
            State result1 = SG_Measurement(state, sgDirections[0]);
            
            // Feed result into second SG
            State result2 = SG_Measurement(result1, sgDirections[1]);
            
            // Count final outcomes
            if (result2.theta < PI / 2) {
                countUpFinal++;
            } else {
                countDownFinal++;
            }
        }

        // Calculate results
        results.particleSum = particleCount;  // All particles are measured
        results.particleCount = particleCount;
        results.upCount = countUpFinal;
        results.downCount = countDownFinal;
        results.throughputPercent = 100.0;
        results.upPercent = (double)countUpFinal / particleCount * 100.0;
        results.downPercent = (double)countDownFinal / particleCount * 100.0;

        emit resultsChanged();
    }
}

void SternGerlachSimulator::analyzeGrid(const QVariantList& grid, 
                                        const QString& initialState,
                                        int particleCount) {
    qDebug() << "Analyzing grid with particle count:" << particleCount;  // Add debug
    
    if (particleCount <= 0) {
        qDebug() << "Invalid particle count:" << particleCount;
        return;
    }

    QVector<char> sgDirections;
    QVector<bool> blockSpinUp;
    
    if (findPattern(grid, sgDirections, blockSpinUp)) {
        qDebug() << "Running simulation with:";
        qDebug() << "Initial state:" << initialState;
        qDebug() << "Particle count:" << particleCount;
        runSimulation(initialState, sgDirections, blockSpinUp, particleCount);
    }
}

QVariantMap SternGerlachSimulator::getResults() const {
    return QVariantMap{
        {"particleSum", results.particleSum},
        {"particleCount", results.particleCount},
        {"throughputPercent", results.throughputPercent},
        {"upCount", results.upCount},
        {"downCount", results.downCount},
        {"upPercent", results.upPercent},
        {"downPercent", results.downPercent}
    };
}
