#ifndef EXTQUERY_H
#define EXTQUERY_H

#include "../query.h"

namespace spunkydbext {

using namespace spunkydb;

using Set = std::unique_ptr<std::unordered_set<std::string>>;

class DefaultQueryResult : public IQueryResult {
public:
    DefaultQueryResult();
    DefaultQueryResult(const Set recordKeys);
    virtual ~DefaultQueryResult() = default;

    const Set recordKeys();
private:
    Set m_recordKeys;
};

}

#endif // EXTQUERY_H
