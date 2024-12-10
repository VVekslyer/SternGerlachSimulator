/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  SternGerlachSimulator.h
 */

#ifndef SternGerlachSimulator_H
#define SternGerlachSimulator_H

#include <QObject>
#include <QVariantMap>
#include <QVector>

// Global State struct
struct State {
    double theta;
    double phi;
};

class SternGerlachSimulator : public QObject {
    Q_OBJECT
    Q_PROPERTY(int particleSum READ particleSum NOTIFY resultsChanged)
    Q_PROPERTY(int particleCount READ particleCount NOTIFY resultsChanged)
    Q_PROPERTY(double throughputPercent READ throughputPercent NOTIFY resultsChanged)
    Q_PROPERTY(double upPercent READ upPercent NOTIFY resultsChanged)
    Q_PROPERTY(double downPercent READ downPercent NOTIFY resultsChanged)
    Q_PROPERTY(int upCount READ upCount NOTIFY resultsChanged)
    Q_PROPERTY(int downCount READ downCount NOTIFY resultsChanged)

public:
    explicit SternGerlachSimulator(QObject *parent = nullptr);

    Q_INVOKABLE void analyzeGrid(const QVariantList& grid, const QString& initialState, int particleCount);
    Q_INVOKABLE QVariantMap getResults() const;

    // Getters
    int particleSum() const { return results.particleSum; }
    int particleCount() const { return results.particleCount; }
    double throughputPercent() const { return results.throughputPercent; }
    double upPercent() const { return results.upPercent; }
    double downPercent() const { return results.downPercent; }
    int upCount() const { return results.upCount; }
    int downCount() const { return results.downCount; }

signals:
    void resultsChanged();

private:
    struct SimResults {
        int particleSum;
        int particleCount;
        int upCount;
        int downCount;
        double throughputPercent;
        double upPercent;
        double downPercent;
    };

    SimResults results;

    // Function declarations using global State struct
    void runSimulation(const QString& initialState, const QVector<char>& sgDirections,
                       const QVector<bool>& blockSpinUp, int particleCount);
    bool findPattern(const QVariantList& grid, QVector<char>& sgDirections, QVector<bool>& blockSpinUp);

    State generateInitialState(const QString& initialState);
    State setState(char direction, bool spinUp);
    State SG_Measurement(const State& input, char direction);
};

#endif
