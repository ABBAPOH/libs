#ifndef BOOLLOCKER_H
#define BOOLLOCKER_H

class BoolLocker
{
public:
    explicit BoolLocker(bool *b);
    ~BoolLocker();

private:
    bool *m_b;
    bool m_oldValue;
};

inline BoolLocker::BoolLocker(bool *b):
    m_b(b)
{
    Q_ASSERT(m_b);
    m_oldValue = *m_b;
    *m_b = !m_oldValue;
}

inline BoolLocker::~BoolLocker()
{
    *m_b = m_oldValue;
}

#endif // BOOLLOCKER_H
