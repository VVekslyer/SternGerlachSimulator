// sterngerlachsimulator.cpp
#include "sterngerlachsimulator.h"
#include <random>
#include <cmath>

const double PI = 3.14159265358979323846;

struct State {
    double theta, phi;
};

SternGerlachSimulator::SternGerlachSimulator(QObject *parent) 
    : QObject(parent) {
    results = SimResults{0, 0, 0, 0, 0.0, 0.0, 0.0};
}

bool SternGerlachSimulator::findPattern(const QVariantList& grid, QVector<char>& sgDirections, 
                                      QVector<bool>& blockSpinUp) {
    const int WIDTH = 16;
    const int HEIGHT = 16;

    // Convert grid to 2D array for easier processing
    QVector<QVector<QString>> grid2D(HEIGHT, QVector<QString>(WIDTH));
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid2D[i][j] = grid[i * WIDTH + j].toString();
        }
    }

    // Scan for patterns in 4-row windows
    for (int startRow = 0; startRow < HEIGHT - 3; startRow++) {
        // Look for SG device pattern (X/Y/Z followed by - and | with output)
        for (int j = 0; j < WIDTH - 2; j++) {
            for (int row = startRow; row < startRow + 4; row++) {
                QString type = grid2D[row][j];
                if (type == "X" || type == "Y" || type == "Z") {
                    // Found an SG device, check for connection pattern
                    if (j + 2 < WIDTH && 
                        grid2D[row][j+1] == "-" && 
                        grid2D[row+1][j+1] == "|" &&
                        grid2D[row][j+2] == "o") {
                        
                        sgDirections.append(type[0].toLatin1());
                        // Check if next row has another SG, determines blocking
                        blockSpinUp.append(row < startRow + 2);
                        return true;
                    }
                }
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
    
    for (int i = 0; i < sgDirections.size(); i++) {
        QVector<int> upDown(2, 0);  // [up_count, down_count]
        
        // Run particles through current SG device
        for (int p = 0; p < remainingParticles; p++) {
            State result = SG_Measurement(state, sgDirections[i]);
            if (result.theta < PI/2) upDown[0]++;
            else upDown[1]++;
        }
        
        // Update remaining particles based on blocking
        remainingParticles = blockSpinUp[i] ? upDown[0] : upDown[1];
        
        // Set state for next device
        state.theta = blockSpinUp[i] ? 0 : PI;
        state.phi = 0;
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
    results.upPercent = 100.0 * finalUp / remainingParticles;
    results.downPercent = 100.0 * finalDown / remainingParticles;
    
    emit resultsChanged();
}

void SternGerlachSimulator::analyzeGrid(const QVariantList& grid, 
                                      const QString& initialState,
                                      int particleCount) {
    QVector<char> sgDirections;
    QVector<bool> blockSpinUp;
    
    if (findPattern(grid, sgDirections, blockSpinUp)) {
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
