#ifndef OPENSTRATEGY_P_H
#define OPENSTRATEGY_P_H

#include "openstrategy.h"

namespace Parts {

class OpenStrategyPrivate
{
public:
    OpenStrategyPrivate() {}

    static OpenStrategyPrivate *instance();

    QList<OpenStrategy *> strategies;

    static void addStrategy(OpenStrategy *strategy);
    static void removeStrategy(OpenStrategy *strategy);
};

} // namespace Parts

#endif // OPENSTRATEGY_P_H
