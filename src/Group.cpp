#include "Group.h"
#include <netcdf.h>
#include "Attribute.h"
//#include "Dimension.h"
//#include "Variable.h"
#include "napi-utils.h"

namespace netcdf4js {

napi_ref Group::constructor;

Group::Group(const int& id_) : id(id_) {}

Group::~Group() {
    napi_delete_reference(env_, wrapper_);
}

extern "C" {
    napi_value Group_Init(napi_env env, napi_value exports) {
        return Group::Init(env, exports);
    }
}

void Group::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
    reinterpret_cast<Group*>(nativeObject)->~Group();
}

napi_value Group::Init(napi_env env, napi_value exports) {
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_METHOD("addVariable", AddVariable),
        DECLARE_NAPI_METHOD("addDimension", AddDimension),
        DECLARE_NAPI_METHOD("addSubgroup", AddSubgroup),
        DECLARE_NAPI_METHOD("addAttribute", AddAttribute),
        DECLARE_NAPI_METHOD("inspect", Inspect),
        DECLARE_NAPI_PROP("id", Group::GetId, nullptr),
        DECLARE_NAPI_PROP("variables", Group::GetVariables, nullptr),
        DECLARE_NAPI_PROP("dimensions", Group::GetDimensions, nullptr),
        DECLARE_NAPI_PROP("unlimited", Group::GetUnlimited, nullptr),
        DECLARE_NAPI_PROP("attributes", Group::GetAttributes, nullptr),
        DECLARE_NAPI_PROP("subgroups", Group::GetSubgroups, nullptr),
        DECLARE_NAPI_PROP("name", Group::GetName, nullptr),
        DECLARE_NAPI_PROP("fullname", Group::GetFullname, nullptr),
    };

    napi_value constructor;

    NAPI_CALL(napi_define_class(
        env,
        "Group", NAPI_AUTO_LENGTH,
        Group::New,
        nullptr,
        13,
        properties,
        &constructor
    ));

    NAPI_CALL(napi_set_named_property(
        env,
        exports,
        "Group",
        constructor
    ));

    return nullptr;
}

napi_value Group::Build(napi_env env, napi_value jsthis, int id) {
    Group* group = new Group(id);
    group->env_ = env;

    if (jsthis == nullptr) {
        NAPI_CALL(napi_create_object(env, &jsthis));
    }

    NAPI_CALL(napi_wrap(env,
        jsthis,
        reinterpret_cast<void*>(group),
        Group::Destructor,
        nullptr,  // finalize_hint
        &group->wrapper_
    ));

    return jsthis;
}

napi_value Group::New(napi_env env, napi_callback_info info) {
    ARGS(1, I32(id))
    return Build(env, jsthis, id);
}

bool Group::get_name(char* name) const {
    int retval = nc_inq_grpname(id, name);
    if (retval != NC_NOERR) {
        napi_throw_error(env_, NULL, nc_strerror(retval));
        return false;
    }
    return true;
}

napi_value Group::AddAttribute(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = args.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(args.Holder());
    if (args.Length() < 3) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    std::string type_str = *v8::String::Utf8Value(
        isolate,
        args[1]);

    int type = get_type(type_str);
    if (type == NC2_ERR) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Unknown variable type", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    Attribute* res = new Attribute(*v8::String::Utf8Value(
                                       isolate,
                                       args[0]),
                                   NC_GLOBAL, obj->id, type);
    res->set_value(args[2]);
    args.GetReturnValue().Set(res->handle());
    */return NULL;
}

napi_value Group::AddSubgroup(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = args.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(args.Holder());
    if (args.Length() < 1) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    int new_id;
    int retval = nc_def_grp(obj->id,
                            *v8::String::Utf8Value(
                                isolate,
                                args[0]),
                            &new_id);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    Group* res = new Group(new_id);
    args.GetReturnValue().Set(res->handle());
    */return NULL;
}

napi_value Group::AddDimension(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = args.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(args.Holder());
    if (args.Length() < 2) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    int len;
    if (std::string(*v8::String::Utf8Value(
            isolate,
            args[1]))
        == "unlimited") {
        len = NC_UNLIMITED;
    } else {
        if (!args[1]->IsUint32()) {
            isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Expecting a positive integer", v8::NewStringType::kNormal).ToLocalChecked()));
            return;
        }
        len = args[1]->Uint32Value(isolate->GetCurrentContext()).ToChecked();
    }
    int new_id;
    int retval = nc_def_dim(obj->id,
                            *v8::String::Utf8Value(
                                isolate,
                                args[0]),
                            len, &new_id);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    Dimension* res = new Dimension(new_id, obj->id);
    args.GetReturnValue().Set(res->handle());
    */return NULL;
}

napi_value Group::AddVariable(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = args.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(args.Holder());
    if (args.Length() < 3) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    std::string type_str = *v8::String::Utf8Value(
        isolate,
        args[1]);
    int type = get_type(type_str);
    if (type == NC2_ERR) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Unknown variable type", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    if (type == NC_STRING) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Unsupported variable type", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    if (!args[2]->IsArray()) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Expecting an array", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    v8::Local<v8::Object> array = args[2]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
    size_t ndims = array->Get(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, "length", v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked()->Uint32Value(isolate->GetCurrentContext()).ToChecked();
    int* dimids = new int[ndims];
    for (size_t i = 0; i < ndims; i++) {
        dimids[i] = array->Get(isolate->GetCurrentContext(), i).ToLocalChecked()->Int32Value(isolate->GetCurrentContext()).ToChecked();
    }
    int new_id;
    int retval = nc_def_var(obj->id,
                            *v8::String::Utf8Value(
                                isolate,
                                args[0]),
                            type, ndims, dimids, &new_id);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] dimids;
        return;
    }
    Variable* res = new Variable(new_id, obj->id);
    args.GetReturnValue().Set(res->handle());
    delete[] dimids;
    */return NULL;
}

napi_value Group::GetId(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    info.GetReturnValue().Set(v8::Integer::New(isolate, obj->id));
    */return NULL;
}

napi_value Group::GetVariables(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    int nvars;
    int retval = nc_inq_varids(obj->id, &nvars, NULL);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    int* var_ids = new int[nvars];
    retval = nc_inq_varids(obj->id, NULL, var_ids);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] var_ids;
        return;
    }
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    char name[NC_MAX_NAME + 1];
    for (int i = 0; i < nvars; ++i) {
        Variable* v = new Variable(var_ids[i], obj->id);
        if (v->get_name(name)) {
            result->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked(), v->handle());
        } else {
            delete[] var_ids;
            return;
        }
    }
    info.GetReturnValue().Set(result);
    delete[] var_ids;
    */return NULL;
}

napi_value Group::GetDimensions(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    int ndims;
    int retval = nc_inq_dimids(obj->id, &ndims, NULL, 0);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    int* dim_ids = new int[ndims];
    retval = nc_inq_dimids(obj->id, NULL, dim_ids, 0);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] dim_ids;
        return;
    }
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    char name[NC_MAX_NAME + 1];
    for (int i = 0; i < ndims; ++i) {
        Dimension* d = new Dimension(dim_ids[i], obj->id);
        if (d->get_name(name)) {
            result->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked(), d->handle());
        } else {
            delete[] dim_ids;
            return;
        }
    }
    info.GetReturnValue().Set(result);
    delete[] dim_ids;
    */return NULL;
}

napi_value Group::GetUnlimited(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    int ndims;
    int retval = nc_inq_unlimdims(obj->id, &ndims, NULL);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    int* dim_ids = new int[ndims];
    retval = nc_inq_unlimdims(obj->id, NULL, dim_ids);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] dim_ids;
        return;
    }
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    char name[NC_MAX_NAME + 1];
    for (int i = 0; i < ndims; ++i) {
        Dimension* d = new Dimension(dim_ids[i], obj->id);
        if (d->get_name(name)) {
            result->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked(), d->handle());
        } else {
            delete[] dim_ids;
            return;
        }
    }
    info.GetReturnValue().Set(result);
    delete[] dim_ids;
    */return NULL;
}

napi_value Group::GetAttributes(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    int natts;
    int retval = nc_inq_natts(obj->id, &natts);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    char name[NC_MAX_NAME + 1];
    for (int i = 0; i < natts; i++) {
        retval = nc_inq_attname(obj->id, NC_GLOBAL, i, name);
        if (retval != NC_NOERR) {
            throw_netcdf_error(isolate, retval);
            return;
        }
        Attribute* a = new Attribute(name, NC_GLOBAL, obj->id);
        result->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked(), a->handle());
    }
    info.GetReturnValue().Set(result);
    */return NULL;
}

napi_value Group::GetSubgroups(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    int ngrps;
    int retval = nc_inq_grps(obj->id, &ngrps, NULL);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    int* grp_ids = new int[ngrps];
    retval = nc_inq_grps(obj->id, NULL, grp_ids);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] grp_ids;
        return;
    }
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    char name[NC_MAX_NAME + 1];
    for (int i = 0; i < ngrps; ++i) {
        Group* g = new Group(grp_ids[i]);
        if (g->get_name(name)) {
            result->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked(), g->handle());
        } else {
            delete[] grp_ids;
            return;
        }
    }
    info.GetReturnValue().Set(result);
    delete[] grp_ids;
    */return NULL;
}

napi_value Group::GetName(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    char name[NC_MAX_NAME + 1];
    if (obj->get_name(name)) {
        info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked());
    }
    */return NULL;
}

napi_value Group::GetFullname(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = info.GetIsolate();
    Group* obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    size_t len;
    int retval = nc_inq_grpname_len(obj->id, &len);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        return;
    }
    char* name = new char[len + 1];
    name[len] = 0;
    retval = nc_inq_grpname_full(obj->id, NULL, name);
    if (retval != NC_NOERR) {
        throw_netcdf_error(isolate, retval);
        delete[] name;
        return;
    }
    info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, name, v8::NewStringType::kNormal).ToLocalChecked());
    delete[] name;
    */return NULL;
}

napi_value Group::Inspect(napi_env env, napi_callback_info info) {
    /*
    v8::Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, "[object Group]", v8::NewStringType::kNormal).ToLocalChecked());
    */return NULL;
}
}  // namespace netcdf4js
