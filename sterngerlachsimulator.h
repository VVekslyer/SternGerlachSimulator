// sterngerlachsimulator.h
#ifndef STERNGERLACHSIMULATOR_H
#define STERNGERLACHSIMULATOR_H

#include <QObject>
#include <QVariantMap>

class SternGerlachSimulator : public QObject {
    Q_OBJECT

public:
    explicit SternGerlachSimulator(QObject *parent = nullptr);

    Q_INVOKABLE void analyzeGrid(const QVariantList& grid, const QString& initialState, int particleCount);
    Q_INVOKABLE QVariantMap getResults() const;

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
