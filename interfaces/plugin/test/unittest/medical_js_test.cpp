/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include <thread>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "medical_napi_utils.h"
#include "medical_native_impl.h"
#include "native_engine.h"
#include "quickjs_native_engine.h"
#include "hilog/log.h"

namespace OHOS {
namespace Sensors {
using namespace testing::ext;
using namespace OHOS::HiviewDFX;
static NativeEngine* g_nativeEngine = nullptr;
namespace {
    using namespace OHOS::HiviewDFX;
    static constexpr HiLogLabel LABEL = {LOG_CORE, 0xD002784, "AfeJsTest"};
}

class AfeJsTest : public testing::Test {
public:
    void SetUp();
    void TearDown();
    AfeJsTest();
    virtual ~AfeJsTest();
protected:
    NativeEngine* engine_;
};

void AfeJsTest::SetUp()
{
    HiLog::Info(LABEL, "%{public}s begin", __func__);
}

void AfeJsTest::TearDown()
{
    HiLog::Info(LABEL, "%{public}s end", __func__);
}

AfeJsTest::AfeJsTest()
{
    JSRuntime* rt = JS_NewRuntime();
    if (rt == nullptr) {
        return ;
    }

    JSContext* ctx = JS_NewContext(rt);
    if (ctx == nullptr) {
        return ;
    }

    js_std_add_helpers(ctx, 0, nullptr);
    g_nativeEngine = new QuickJSNativeEngine(rt, ctx, 0); // default instance id 0
    engine_ = g_nativeEngine;

}

AfeJsTest::~AfeJsTest() {
    delete g_nativeEngine;
    g_nativeEngine = nullptr;
}

/*
 * @tc.name: IsMatchType_001
 * @tc.desc: Match napi_number
 * @tc.type: FUNC
 * @tc.require:SR000G54IU
 */
HWTEST_F(AfeJsTest, IsMatchType_001, TestSize.Level1)
{
    napi_env env = (napi_env)engine_;
    uint32_t cppValue = UINT32_MAX;
    napi_value napiValue = nullptr;
    napi_create_uint32(env, cppValue, &napiValue);

    bool ret = IsMatchType(napiValue, napi_number, env);
    ASSERT_EQ(ret, true);
}

/*
 * @tc.name: IsMatchType_002
 * @tc.desc: Match napi_string
 * @tc.type: FUNC
 * @tc.require:AR000G54IV
 */
HWTEST_F(AfeJsTest, IsMatchType_002, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    const char cppValue[] = "中文,English,123456,!@#";
    size_t cppValueStrLength = strlen(cppValue);
    napi_value napiValue = nullptr;
    napi_create_string_utf8(env, cppValue, cppValueStrLength, &napiValue);

    // call function
    bool ret = IsMatchType(napiValue, napi_string, env);
    ASSERT_EQ(ret, true);
}

/*
 * @tc.name: IsMatchType_003
 * @tc.desc: Match napi_symbol
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, IsMatchType_003, TestSize.Level1)
{
    // Construction parameters
    napi_env env = (napi_env)engine_;
    const char testStr[] = "testSymbol";
    napi_value result = nullptr;
    napi_create_string_latin1(env, testStr, strlen(testStr), &result);
    napi_value symbolVal = nullptr;
    napi_create_symbol(env, result, &symbolVal);

    // call function
    bool ret = IsMatchType(symbolVal, napi_symbol, env);
    ASSERT_EQ(ret, true);
}

/*
 * @tc.name: IsMatchType_004
 * @tc.desc: Match napi_function
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, IsMatchType_004, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    const char* tetScriptStr = "new Date();";
    napi_value testScript = nullptr;
    napi_create_string_utf8(env, tetScriptStr, strlen(tetScriptStr), &testScript);
    napi_value date = nullptr;
    napi_run_script(env, testScript, &date);
    napi_value getTimeFunc = nullptr;
    napi_get_named_property(env, date, "getTime", &getTimeFunc);

    // call function
    bool ret = IsMatchType(getTimeFunc, napi_function, env);
    ASSERT_EQ(ret, true);
}

/*
 * @tc.name: IsMatchType_005
 * @tc.desc: Match napi_boolean
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, IsMatchType_005, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    napi_value result = nullptr;
    napi_get_boolean(env, true, &result);

    // call function
    bool ret = IsMatchType(result, napi_boolean, env);
    ASSERT_EQ(ret, true);
}

/*
 * @tc.name: GetNapiInt32_001
 * @tc.desc: change int32_t[INT32_MAX] to napi_value
 * @tc.type: FUNC
 * @tc.require: SR000FU58Q
 */
HWTEST_F(AfeJsTest, GetNapiInt32_001, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    int32_t number = INT32_MAX;
    
    // call function
    napi_value value = GetNapiInt32(number, env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: GetNapiInt32_002
 * @tc.desc: change int32_t[INT32_MIN] to napi_value
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, GetNapiInt32_002, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    int32_t number = INT32_MIN;

    // call function
    napi_value value = GetNapiInt32(number, env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: GetNapiInt32_003
 * @tc.desc: change int32_t[true] to napi_value
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, GetNapiInt32_003, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)true;
    
    // call function
    napi_value value = GetNapiInt32(number, env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: GetNapiInt32_004
 * @tc.desc: change int32_t[true] to napi_value
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, GetNapiInt32_004, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)false;
    
    // call function
    napi_value value = GetNapiInt32(number, env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: GetNapiInt32_005
 * @tc.desc: change int32_t[char] to napi_value
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, GetNapiInt32_005, TestSize.Level1)
{
    // Construction parameters    
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)'a';
    
    // call function
    napi_value value = GetNapiInt32(number, env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: NapiGetNamedProperty_001
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, NapiGetNamedProperty_001, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    const char* tetScriptStr = "new Date();";
    napi_value testScript = nullptr;
    napi_create_string_utf8(env, tetScriptStr, strlen(tetScriptStr), &testScript);
    napi_value date = nullptr;
    napi_run_script(env, testScript, &date);

    // call function
    napi_value value = NapiGetNamedProperty(date, "getTime", env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_function);
}

/*
 * @tc.name: NapiGetNamedProperty_002
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, NapiGetNamedProperty_002, TestSize.Level1)
{
    // 定义变量 赋值
    napi_env env = (napi_env)engine_;
    int32_t status=101;
    napi_value message;
    napi_create_int32(env, status, &message);

    // 定义变量为对象
    napi_value result;
    napi_create_object(env, &result);

    // 赋值给object
    napi_set_named_property(env, result, "code", message);

    napi_value value = NapiGetNamedProperty(result, "code", env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_number);
}

/*
 * @tc.name: NapiGetNamedProperty_003
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000FU73V
 */
HWTEST_F(AfeJsTest, NapiGetNamedProperty_003, TestSize.Level1)
{
    // 定义变量 赋值
    napi_env env = (napi_env)engine_;
    const char status[] = "type001";
    napi_value message;
    napi_create_string_utf8(env, status, strlen(status), &message);

    // 定义变量为对象
    napi_value result;
    napi_create_object(env, &result);

    // 赋值给object
    napi_set_named_property(env, result, "code", message);

    napi_value value = NapiGetNamedProperty(result, "code", env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_string);
}

/*
 * @tc.name: NapiGetNamedProperty_004
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, NapiGetNamedProperty_004, TestSize.Level1)
{
    // 定义变量 赋值
    napi_env env = (napi_env)engine_;
    napi_value message;
    napi_get_boolean(env, true, &message);

    // 定义变量为对象
    napi_value result;
    napi_create_object(env, &result);

    // 赋值给object
    napi_set_named_property(env, result, "code", message);


    napi_value value = NapiGetNamedProperty(result, "code", env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_boolean);
}

/*
 * @tc.name: NapiGetNamedProperty_005
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, NapiGetNamedProperty_005, TestSize.Level1)
{
    // 定义变量 赋值
    napi_env env = (napi_env)engine_;
    const char testStr[] = "testSymbol";
    napi_value resultSymbol = nullptr;
    napi_create_string_latin1(env, testStr, strlen(testStr), &resultSymbol);
    napi_value message = nullptr;
    napi_create_symbol(env, resultSymbol, &message);

    // 定义变量为对象
    napi_value result;
    napi_create_object(env, &result);

    // 赋值给object
    napi_set_named_property(env, result, "code", message);

    napi_value value = NapiGetNamedProperty(result, "code", env);
    napi_valuetype ret;
    napi_typeof(env, value, &ret);
    ASSERT_EQ(ret, napi_symbol);
}
/*
 * @tc.name: GetCppInt32_001
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt32_001, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int32_t number = INT32_MAX;
    napi_value value;
    napi_create_int32(env, number, &value);

    // call function
    int32_t ret = GetCppInt32(value, env);
    ASSERT_EQ(ret, (int32_t)INT32_MAX);
}

/*
 * @tc.name: GetCppInt32_002
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt32_002, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int32_t number = INT32_MIN;
    napi_value value;
    napi_create_int32(env, number, &value);

    // call function
    int32_t ret = GetCppInt32(value, env);
    ASSERT_EQ(ret, (int32_t)INT32_MIN);
}

/*
 * @tc.name: GetCppInt32_003
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt32_003, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)true;
    napi_value value;
    napi_create_int32(env, number, &value);

    // call function
    int32_t ret = GetCppInt32(value, env);
    ASSERT_EQ(ret, (int32_t)true);
}

/*
 * @tc.name: GetCppInt32_004
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt32_004, TestSize.Level1)
{
    // Construction parameters
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)false;
    napi_value value;
    napi_create_int32(env, number, &value);

    // call function
    int32_t ret = GetCppInt32(value, env);
    ASSERT_EQ(ret, (int32_t)false);
}
/*
 * @tc.name: GetCppInt32_005
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt32_005, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int32_t number = (int32_t)'b';
    napi_value value;
    napi_create_int32(env, number, &value);

    // call function
    int32_t ret = GetCppInt32(value, env);
    ASSERT_EQ(ret, (int32_t)'b');
}

/*
 * @tc.name: GetCppInt64_001
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt64_001, TestSize.Level1)
{
    // Construction parameters  
    int64_t tmpInt64Max = 9007199254740991;//9223372036854775807;9007199254740992
    napi_env env = (napi_env)engine_;
    int64_t number = tmpInt64Max;//(int64_t)INT64_MAX;
    napi_value value;
    napi_create_int64(env, number, &value);

    // call function
    int64_t ret = GetCppInt64(value, env);
    ASSERT_EQ(ret, tmpInt64Max);
}

/*
 * @tc.name: GetCppInt64_002
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt64_002, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int64_t number = (int64_t)INT64_MIN;
    napi_value value;
    napi_create_int64(env, number, &value);

    // call function
    int64_t ret = GetCppInt64(value, env);
    ASSERT_EQ(ret, (int64_t)INT64_MIN);
}

/*
 * @tc.name: GetCppInt64_003
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt64_003, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int64_t number = (int64_t)true;
    napi_value value;
    napi_create_int64(env, number, &value);


    // call function
    int64_t ret = GetCppInt64(value, env);
    ASSERT_EQ(ret, (int64_t)true);
}

/*
 * @tc.name: GetCppInt64_004
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt64_004, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int64_t number = (int64_t)false;
    napi_value value;
    napi_create_int64(env, number, &value);

    // call function
    int64_t ret = GetCppInt64(value, env);
    ASSERT_EQ(ret, (int64_t)false);
}
/*
 * @tc.name: GetCppInt64_005
 * @tc.desc: Parse function in Json
 * @tc.type: FUNC
 * @tc.require: AR000G54IV
 */
HWTEST_F(AfeJsTest, GetCppInt64_005, TestSize.Level1)
{
    // Construction parameters  
    napi_env env = (napi_env)engine_;
    int64_t number = (int64_t)'c';
    napi_value value;
    napi_create_int64(env, number, &value);

    // call function
    int64_t ret = GetCppInt64(value, env);
    ASSERT_EQ(ret, (int64_t)'c');
}
}  // namespace Sensors
}  // namespace OHOS