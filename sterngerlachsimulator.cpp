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

    // Define all possible 3-SG patterns (bottom to top)
    Pattern pattern1_XZY = {  // Original
        {
            ". . . . . . . . Y . - . o",
            ". . . . Z . . . . . | . .",
            "X . . . . . . . . . . . ."
        },
        3, 13
    };

    Pattern pattern2_XZY_alt = {  // Original alternative output
        {
            ". . . . . . . . Y . | . .",
            ". . . . Z . . . . . - . o",
            "X . . . . . . . . . . . ."
        },
        3, 13
    };

    // Add new 3-SG variations
    Pattern pattern_ZXY = {
        {
            ". . . . . . . . Y . - . o",
            ". . . . X . . . . . | . .",
            "Z . . . . . . . . . . . ."
        },
        3, 13
    };

    Pattern pattern_YXZ = {
        {
            ". . . . . . . . Z . - . o",
            ". . . . X . . . . . | . .",
            "Y . . . . . . . . . . . ."
        },
        3, 13
    };

    Pattern pattern_YZX = {
        {
            ". . . . . . . . X . - . o",
            ". . . . Z . . . . . | . .",
            "Y . . . . . . . . . . . ."
        },
        3, 13
    };

    Pattern pattern_XYZ = {
        {
            ". . . . . . . . Z . - . o",
            ". . . . Y . . . . . | . .",
            "X . . . . . . . . . . . ."
        },
        3, 13
    };

    // 2-SG patterns
    Pattern pattern_XZ = {  // Original 2-SG
        {
            ". . . . X . - . o",
            "Z . . . . . | . ."
        },
        2, 9
    };

    Pattern pattern_ZX = {
        {
            ". . . . X . - . o",
            "Z . . . . . | . ."
        },
        2, 9
    };

    Pattern pattern_XY = {
        {
            ". . . . Y . - . o",
            "X . . . . . | . ."
        },
        2, 9
    };

    Pattern pattern_YX = {
        {
            ". . . . X . - . o",
            "Y . . . . . | . ."
        },
        2, 9
    };

    Pattern pattern_YZ = {
        {
            ". . . . Z . - . o",
            "Y . . . . . | . ."
        },
        2, 9
    };

    Pattern pattern_ZY = {
        {
            ". . . . Y . - . o",
            "Z . . . . . | . ."
        },
        2, 9
    };

    // Store all patterns in list
    QVector<Pattern> patterns = {
        // 3-SG patterns
        pattern1_XZY, pattern2_XZY_alt, 
        pattern_ZXY, pattern_YXZ,
        pattern_YZX, pattern_XYZ,
        // 2-SG patterns
        pattern_XZ, pattern_ZX,
        pattern_XY, pattern_YX,
        pattern_YZ, pattern_ZY
    };

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
                    if (pat.patternData == pattern_XZ.patternData || pat.patternData == pattern_ZX.patternData ||
                        pat.patternData == pattern_XY.patternData || pat.patternData == pattern_YX.patternData ||
                        pat.patternData == pattern_YZ.patternData || pat.patternData == pattern_ZY.patternData) {
                        // Handling the new pattern (2 SG devices)
                        sgDirections = {pat.patternData[1][0].toLatin1(), pat.patternData[0][8].toLatin1()};  // Order: bottom to top
                        blockSpinUp = {true};       // Only one blocking decision needed
                                                    // between the two SG devices
                    } else {
                        // Existing patterns (3 SG devices)
                        sgDirections = {pat.patternData[2][0].toLatin1(), pat.patternData[1][4].toLatin1(), pat.patternData[0][8].toLatin1()};  // Order: bottom to top
                        blockSpinUp = {true, true, false}; // Corresponding spin states

                        if (pat.patternData == pattern2_XZY_alt.patternData) {
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
    results.particleCount = particleCount;
    
    int countUpFinal = 0, countDownFinal = 0;
    int remainingParticles = 0;

    // Process each particle through the SG sequence
    for (int i = 0; i < particleCount; i++) {
        bool particleBlocked = false;
        State currentState = state;

        // Process through each SG device in sequence
        for (int sg = 0; sg < sgDirections.size() - 1; sg++) {
            // Measure current SG
            State result = SG_Measurement(currentState, sgDirections[sg]);
            bool isSpinUp = (result.theta < PI / 2);

            // Check if particle is blocked
            if (blockSpinUp[sg] == isSpinUp) {
                particleBlocked = true;
                break;
            }

            // Prepare state for next measurement
            currentState = setState(sgDirections[sg], isSpinUp);
        }

        // If particle wasn't blocked, measure final SG
        if (!particleBlocked) {
            State finalResult = SG_Measurement(currentState, 
                              sgDirections[sgDirections.size()-1]);
            
            remainingParticles++;
            if (finalResult.theta < PI / 2) {
                countUpFinal++;
            } else {
                countDownFinal++;
            }
        }
    }

    // Calculate results
    results.particleSum = remainingParticles;
    results.upCount = countUpFinal;
    results.downCount = countDownFinal;
    results.throughputPercent = (double)remainingParticles / particleCount * 100.0;
    results.upPercent = (double)countUpFinal / particleCount * 100.0;
    results.downPercent = (double)countDownFinal / particleCount * 100.0;

    emit resultsChanged();
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
