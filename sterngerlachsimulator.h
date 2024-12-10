// sterngerlachsimulator.h
#ifndef STERNGERLACHSIMULATOR_H
#define STERNGERLACHSIMULATOR_H

#include <QObject>
#include <QVariantMap>

class SternGerlachSimulator : public QObject {
    Q_OBJECT
    Q_PROPERTY(int particleSum READ particleSum NOTIFY resultsChanged)
    Q_PROPERTY(int particleCount READ particleCount NOTIFY resultsChanged)
    Q_PROPERTY(double throughputPercent READ throughputPercent NOTIFY resultsChanged)
    Q_PROPERTY(double upPercent READ upPercent NOTIFY resultsChanged)
    Q_PROPERTY(double downPercent READ downPercent NOTIFY resultsChanged)

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
    void runSimulation(const QString& initialState, const QVector<char>& sgDirections,
                       const QVector<bool>& blockSpinUp, int particleCount);
    bool findPattern(const QVariantList& grid, QVector<char>& sgDirections, QVector<bool>& blockSpinUp);
};

#endif
