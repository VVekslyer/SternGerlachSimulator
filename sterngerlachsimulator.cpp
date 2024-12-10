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

struct State {
    double theta, phi;
};

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
    const int WIDTH = 16;
    const int HEIGHT = 16;
    const int PATTERN_HEIGHT = 3;
    const int PATTERN_WIDTH = 13;

    // Clear and pre-allocate vectors with extra space
    sgDirections.clear();
    blockSpinUp.clear();
    sgDirections.resize(16);  // Larger size
    blockSpinUp.resize(16);   // Larger size

    // The expected pattern we're looking for
    const QString pattern[PATTERN_HEIGHT][PATTERN_WIDTH] = {
        {".", ".", ".", ".", ".", ".", ".", ".", "Y", ".", "-", ".", "o"},
        {".", ".", ".", ".", "Z", ".", ".", ".", ".", ".", "|", ".", "."},
        {"X", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", ".", "."}
    };

    // Convert incoming grid to 2D array
    QVector<QVector<QString>> currentGrid(HEIGHT, QVector<QString>(WIDTH));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            currentGrid[i][j] = grid[i * WIDTH + j].toString();
        }
    }

    // Try each possible position in the grid
    for (int i = 0; i <= HEIGHT - PATTERN_HEIGHT; i++) {
        for (int j = 0; j <= WIDTH - PATTERN_WIDTH; j++) {
            bool matches = true;
            
            // Check if pattern matches at current position
            for (int pi = 0; pi < PATTERN_HEIGHT && matches; pi++) {
                for (int pj = 0; pj < PATTERN_WIDTH && matches; pj++) {
                    if (currentGrid[i + pi][j + pj] != pattern[pi][pj]) {
                        matches = false;
                    }
                }
            }

            if (matches) {
                // Resize vectors to exactly 4 elements (3 SG devices + 1 final measurement)
                sgDirections.resize(4);
                blockSpinUp.resize(3);  // Only 3 blocking decisions needed
                
                // Set values in order (bottom to top)
                sgDirections[0] = 'X';
                sgDirections[1] = 'Z';
                sgDirections[2] = 'Y';
                sgDirections[3] = 'Y';  // Final measurement direction
                
                blockSpinUp[0] = true;
                blockSpinUp[1] = true;
                blockSpinUp[2] = false;

                qDebug() << "Vector sizes after pattern match:" << sgDirections.size() << blockSpinUp.size();
                return true;
            }
        }
    }
    
    return false;
}

State generateInitialState(const QString& initialState) {
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
        // Parse custom state format: "custom,theta,phi"
        QStringList parts = initialState.split(',');
        if (parts.size() == 3) {
            state.theta = parts[1].toDouble();
            state.phi = parts[2].toDouble();
        }
    }
    return state;
}

State SG_Measurement(const State& input, char direction) {
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

void SternGerlachSimulator::runSimulation(const QString& initialState, 
                                          const QVector<char>& sgDirections,
                                          const QVector<bool>& blockSpinUp, 
                                          int particleCount) {
    State state = generateInitialState(initialState);
    int remainingParticles = particleCount;
    
    qDebug() << "\nSimulation Configuration:";
    QString config = "Initial state: " + initialState;
    
    // First device (X) just shows arrow
    config += " --> SG-" + QString(sgDirections[0]);
    
    // Only process the next 2 devices (Z and Y)
    for (int i = 1; i < 3; i++) {
        config += (blockSpinUp[i-1] ? " --SpinUp--> " : " --SpinDown--> ");
        config += "SG-" + QString(sgDirections[i]);
    }
    
    config += " ---> Final Output";
    qDebug() << config;
    
    // Rest of simulation code remains the same...
    // For each SG device
    for (int i = 0; i < sgDirections.size(); i++) {
        QVector<int> upDown(2, 0);
        
        // Run particles through current SG device
        for (int p = 0; p < remainingParticles; p++) {
            State result = SG_Measurement(state, sgDirections[i]);
            if (result.theta < PI/2) {
                upDown[0]++;  // Count spin up
            } else {
                upDown[1]++;  // Count spin down
            }
        }
        
        // Update remaining particles if not last device
        if (i < blockSpinUp.size()) {
            // If blockSpinUp is true, we block spin up particles (keep down)
            // If blockSpinUp is false, we block spin down particles (keep up)
            remainingParticles = blockSpinUp[i] ? upDown[1] : upDown[0];
            state.theta = blockSpinUp[i] ? PI : 0;
            state.phi = 0;
        }
    }
    
    // Store final results
    results.particleSum = remainingParticles;
    results.particleCount = particleCount;
    results.throughputPercent = 100.0 * remainingParticles / particleCount;
    
    // Run final measurement
    int finalUp = 0, finalDown = 0;
    for (int i = 0; i < remainingParticles; i++) {
        State result = SG_Measurement(state, sgDirections.last());
        if (result.theta < PI/2) finalUp++;
        else finalDown++;
    }
    
    results.upCount = finalUp;
    results.downCount = finalDown;
    // results.upPercent = 100.0 * finalUp / remainingParticles;
    // results.downPercent = 100.0 * finalDown / remainingParticles;
    results.upPercent = 100.0 * results.upCount / results.particleCount;
    results.downPercent = 100.0 * results.downCount / results.particleCount;
    
    // Format output like the example
    qDebug() << QString("Total particle throughput: %1/%2 (%3% of original amount)")
                .arg(results.particleSum)
                .arg(results.particleCount)
                .arg(results.throughputPercent, 0, 'f', 0);
                
    qDebug() << QString("Particles measured with spin up: %1 (%2% of final output, %3% of original amount)")
                .arg(results.upCount)
                .arg(100.0 * finalUp / remainingParticles, 0, 'f', 0)
                .arg(results.upPercent, 0, 'f', 0);
                
    qDebug() << QString("Particles measured with spin down: %1 (%2% of final output, %3% of original amount)")
                .arg(results.downCount)
                .arg(100.0 * finalDown / remainingParticles, 0, 'f', 0)
                .arg(results.downPercent, 0, 'f', 0);
    
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
