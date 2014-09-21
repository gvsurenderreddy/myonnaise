#ifndef INPUTTHREAD_H
#define INPUTTHREAD_H

#include <QThread>
#include <vector>
#include <set>

class InputThread : public QThread
{
    Q_OBJECT
public:
    explicit InputThread(QObject *parent = 0);

    virtual void run();

    void addModifierKey(int keyCode);

signals:
    void sigOnKeyPressed(int keys, bool state);

public slots:
    void onKeysUpdate();

private:
    std::vector<bool> m_keyMap;
    std::set<int> m_keyMod;
};

#endif // INPUTTHREAD_H
