#pragma once

#include <json-c/json.h>

namespace util {

class cJSONDocument {
public:
  explicit cJSONDocument(json_object* _pJSONObject) : pJSONObject(_pJSONObject) {}
  ~cJSONDocument() { if (pJSONObject != nullptr) json_object_put(pJSONObject); }

  bool IsValid() const { return (pJSONObject != nullptr); }

  const json_object* Get() const { return pJSONObject; }
  json_object* Get() { return pJSONObject; }

private:
  json_object* pJSONObject;
};

}
