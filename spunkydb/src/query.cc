#include "query.h"
#include "extensions/extquery.h"

#include <string>

using namespace spunkydb;
using namespace spunkydbext;

class BucketQuery::Impl {
public:
    Impl(std::string& bucket);
    ~Impl() = default;
    std::string m_bucket;
};

BucketQuery::Impl::Impl(std::string& bucket) : m_bucket(bucket) {
    ;
}

BucketQuery::BucketQuery(std::string& bucket) : mImpl(std::make_unique<Impl>(bucket)) {
    ;
}

BucketQuery::~BucketQuery() {
    ;
}

std::string BucketQuery::bucket() const {
    return mImpl->m_bucket;
}

DefaultQueryResult::DefaultQueryResult() : m_recordKeys() {
    ;
}

DefaultQueryResult::DefaultQueryResult(std::unique_ptr<std::unordered_set<std::string>> recordKeys)
    : m_recordKeys(std::move(recordKeys)) {
    ;
}

const std::unique_ptr<std::unordered_set<std::string>> DefaultQueryResult::recordKeys() {
  return std::move(m_recordKeys);
}
