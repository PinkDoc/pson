#### pson

pson is a lightweight parser and it support six type, null , bool, number, string, array, object, and it can parse the encoding of UTF-8.

It's fast, small and it's simple to use.

You just need to `#include "pson.hpp"` to your projects~

#### usage

```
    using namespace pson;
    std::string json = "{\n"
                       "  \"pink\" : {\n"
                       "    \"ktouch\" : \"phone\",\n"
                       "    \"xiaomi\" : \"phone\"\n"
                       "  },\n"
                       "  \"computer\" : [\"惠普\", 100.0, false, true],\n"
                       "  \"age\" : 20,\n"
                       "  \"best code\" : \"non\",\n"
                       "  \"man\" : true\n"
                       "}";

    Value v;
    v.parse(json);
    
    Value v_hello{"hello"};
    Value v_false{false};
    Value v_number(1.2);
    
    assert(v.is<Object>() == true);
    assert(v["computer"][0].as<String>() == "惠普");
    assert(v["pink"]["ktouch"].as_string() == "phone");
    assert(v["computer"][1].as_number() == 100.0);
    assert(v["man"].as<Bool>() == true);

    Value cp_v(v);  // Copy
    Value mv_c(std::move(v)); // Move
    
    // Use array, or object to move and copy
    Value a(JSON_ARRAY), o(JSON_OBJECT);
    a.push_back(std::move(v));     // Move
    o.insert("This is copy", v);   // Copy
    
    // To string
    std::string tostr = a.print();
    
    
    
```

hf!


#### TODO

fix v1
