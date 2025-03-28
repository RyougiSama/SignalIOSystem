#pragma once

/*
    This class is used to manage signal data and its related operations.
*/

#include <QObject>

class SignalModel : public QObject
{
    Q_OBJECT

public:
    SignalModel(QObject *parent);
    ~SignalModel();
};
