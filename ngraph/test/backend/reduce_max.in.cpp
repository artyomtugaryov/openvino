//*****************************************************************************
// Copyright 2017-2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

#include "gtest/gtest.h"
#include "ngraph/ngraph.hpp"
#include "util/engine/test_engines.hpp"
#include "util/test_case.hpp"
#include "util/test_control.hpp"

using namespace std;
using namespace ngraph;

static string s_manifest = "${MANIFEST}";
using TestEngine = test::ENGINE_CLASS_NAME(${BACKEND_NAME});

// ----------------------- keep dims = false ----------------------- //

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_to_scalar)
{
    Shape shape{2, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape);
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1, 2, 3, 4};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>({4});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_to_scalar_int8)
{
    Shape shape{2, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::i8, shape);
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<int8_t> a{1, 2, 3, 4};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<int8_t>({a});
    test_case.add_expected_output<int8_t>({4});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_columns)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1, 2, 3, 4, 5, 6};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {5, 6});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_rows)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1, 2, 3, 4, 5, 6};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {2, 4, 6});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_rows_int32)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<int32_t> a{1, 2, 3, 4, 5, 6};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<int32_t>({a});
    test_case.add_expected_output<int32_t>(shape_rt, {2, 4, 6});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_rows_zero)
{
    Shape shape_a{3, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt,
                                         {-std::numeric_limits<float>::infinity(),
                                          -std::numeric_limits<float>::infinity(),
                                          -std::numeric_limits<float>::infinity()});
    test_case.run();
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_rows_zero_int32)
{
    Shape shape_a{3, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i32, shape_a);
    copy_data(a, vector<int32_t>{});
    auto result = backend->create_tensor(element::Type_t::i32, shape_rt);
    copy_data(result, vector<int32_t>({3, 3, 3}));

    int32_t minval = std::numeric_limits<int32_t>::has_infinity
                         ? -std::numeric_limits<int32_t>::infinity()
                         : std::numeric_limits<int32_t>::min();

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int32_t>{minval, minval, minval}), read_vector<int32_t>(result));
    EXPECT_EQ((vector<int32_t>{}), read_vector<int32_t>(a));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_cols_zero)
{
    // Now the reduction (g(x:float32[2,2],y:float32[]) = reduce(x,y,f,axes={})).
    Shape shape_a{0, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3, 3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity(),
                             -std::numeric_limits<float>::infinity()}),
              read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_vector_zero)
{
    Shape shape_a{0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity()}), read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_to_scalar_zero_by_zero)
{
    Shape shape_a{0, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity()}), read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_matrix_most_sig)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {19, 20, 21, 22, 23, 24, 25, 26, 27});
    test_case.run(MIN_FLOAT_TOLERANCE_BITS);
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_matrix_least_sig)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 2);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {3, 6, 9, 12, 15, 18, 21, 24, 27});
    test_case.run(MIN_FLOAT_TOLERANCE_BITS);
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_vector)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {25.0f, 26.0f, 27.0f});
    test_case.run(MIN_FLOAT_TOLERANCE_BITS);
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_scalar)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    // Create some tensors for input/output
    std::vector<float> a{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                         13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1};

    auto test_case = test::TestCase<TestEngine>(f);
    test_case.add_input<float>({a});
    test_case.add_expected_output<float>(shape_rt, {14.0f});
    test_case.run(MIN_FLOAT_TOLERANCE_BITS);
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_scalar_int32)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i32, shape_a);
    copy_data(a, vector<int32_t>{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1});
    auto result = backend->create_tensor(element::Type_t::i32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int32_t>{14}), read_vector<int32_t>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_to_scalar_double)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f64, shape_a);
    Shape shape_rt{};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f64, shape_a);
    copy_data(a, vector<double>{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1});
    auto result = backend->create_tensor(element::Type_t::f64, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<double>{14}), read_vector<double>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_3d_eliminate_zero_dim)
{
    Shape shape_a{3, 0, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    // Overwrite the initial result vector to make sure we're not just coincidentally getting the
    // right value.
    copy_data(result, vector<float>{2112, 2112, 2112, 2112, 2112, 2112});

    float mi = -std::numeric_limits<float>::infinity();

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{mi, mi, mi, mi, mi, mi}), read_vector<float>(result));
}

// ----------------------- keep dims = true ----------------------- //

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_to_scalar)
{
    Shape shape{2, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape);
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape);
    copy_data(a, vector<float>{1, 2, 3, 4});
    auto result = backend->create_tensor(element::Type_t::f32, Shape{1, 1});

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{4}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_to_scalar_int8)
{
    Shape shape{2, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::i8, shape);
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i8, shape);
    copy_data(a, vector<int8_t>{1, 2, 3, 4});
    auto result = backend->create_tensor(element::Type_t::i8, Shape{1, 1});

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int8_t>{4}), read_vector<int8_t>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_columns)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{5, 6}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_rows)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{2, 4, 6}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_rows_int32)
{
    Shape shape_a{3, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{3, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i32, shape_a);
    copy_data(a, vector<int32_t>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_tensor(element::Type_t::i32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int32_t>{2, 4, 6}), read_vector<int32_t>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_rows_zero)
{
    Shape shape_a{3, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3, 3, 3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity(),
                             -std::numeric_limits<float>::infinity(),
                             -std::numeric_limits<float>::infinity()}),
              read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_rows_zero_int32)
{
    Shape shape_a{3, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{3, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i32, shape_a);
    copy_data(a, vector<int32_t>{});
    auto result = backend->create_tensor(element::Type_t::i32, shape_rt);
    copy_data(result, vector<int32_t>({3, 3, 3}));

    int32_t minval = std::numeric_limits<int32_t>::has_infinity
                         ? -std::numeric_limits<int32_t>::infinity()
                         : std::numeric_limits<int32_t>::min();

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int32_t>{minval, minval, minval}), read_vector<int32_t>(result));
    EXPECT_EQ((vector<int32_t>{}), read_vector<int32_t>(a));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_cols_zero)
{
    // Now the reduction (g(x:float32[2,2],y:float32[]) = reduce(x,y,f,axes={})).
    Shape shape_a{0, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3, 3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity(),
                             -std::numeric_limits<float>::infinity()}),
              read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_vector_zero)
{
    Shape shape_a{0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity()}), read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_to_scalar_zero_by_zero)
{
    Shape shape_a{0, 0};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);
    copy_data(result, vector<float>({3}));

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{-std::numeric_limits<float>::infinity()}), read_vector<float>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_matrix_most_sig)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 3, 3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{19, 20, 21, 22, 23, 24, 25, 26, 27}),
                                  read_vector<float>(result),
                                  MIN_FLOAT_TOLERANCE_BITS));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_matrix_least_sig)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 3, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 2);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{3, 6, 9, 12, 15, 18, 21, 24, 27}),
                                  read_vector<float>(result),
                                  MIN_FLOAT_TOLERANCE_BITS));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_vector)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 1, 3};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{2}, vector<int32_t>{0, 1});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{25.0f, 26.0f, 27.0f}),
                                  read_vector<float>(result),
                                  MIN_FLOAT_TOLERANCE_BITS));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_scalar)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{1, 1, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                               13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f(
        (vector<float>{14.0f}), read_vector<float>(result), MIN_FLOAT_TOLERANCE_BITS));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_scalar_int32)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::i32, shape_a);
    Shape shape_rt{1, 1, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::i32, shape_a);
    copy_data(a, vector<int32_t>{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1});
    auto result = backend->create_tensor(element::Type_t::i32, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<int32_t>{14}), read_vector<int32_t>(result));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_to_scalar_double)
{
    Shape shape_a{3, 3, 3};
    auto A = make_shared<op::Parameter>(element::Type_t::f64, shape_a);
    Shape shape_rt{1, 1, 1};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{3}, vector<int32_t>{0, 1, 2});
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f64, shape_a);
    copy_data(a, vector<double>{1,  2,  3,  4,  5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                                13, 12, 11, 10, 9, 8, 7, 6, 5, 4,  3,  2,  1});
    auto result = backend->create_tensor(element::Type_t::f64, shape_rt);

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<double>{14}), read_vector<double>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_3d_eliminate_zero_dim)
{
    Shape shape_a{3, 0, 2};
    auto A = make_shared<op::Parameter>(element::Type_t::f32, shape_a);
    Shape shape_rt{3, 1, 2};
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}");

    // Create some tensors for input/output
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{});
    auto result = backend->create_tensor(element::Type_t::f32, shape_rt);

    // Overwrite the initial result vector to make sure we're not just coincidentally getting the
    // right value.
    copy_data(result, vector<float>{2112, 2112, 2112, 2112, 2112, 2112});

    float mi = -std::numeric_limits<float>::infinity();

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_EQ((vector<float>{mi, mi, mi, mi, mi, mi}), read_vector<float>(result));
}

// Dynamic

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_columns_dynamic)
{
    auto A = make_shared<op::Parameter>(element::Type_t::f32, PartialShape::dynamic());
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}", true);

    // Create some tensors for input/output
    Shape shape_a{3, 2};
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_dynamic_tensor(element::Type_t::f32, PartialShape::dynamic());

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{5, 6}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_matrix_rows_dynamic)
{
    auto A = make_shared<op::Parameter>(element::Type_t::f32, PartialShape::dynamic());
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, false), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}", true);

    // Create some tensors for input/output
    Shape shape_a{3, 2};
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_dynamic_tensor(element::Type_t::f32, PartialShape::dynamic());

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{2, 4, 6}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_columns_dynamic)
{
    auto A = make_shared<op::Parameter>(element::Type_t::f32, PartialShape::dynamic());
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 0);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}", true);

    // Create some tensors for input/output
    Shape shape_a{3, 2};
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_dynamic_tensor(element::Type_t::f32, PartialShape::dynamic());

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{5, 6}), read_vector<float>(result)));
}

NGRAPH_TEST(${BACKEND_NAME}, reduce_max_keep_matrix_rows_dynamic)
{
    auto A = make_shared<op::Parameter>(element::Type_t::f32, PartialShape::dynamic());
    auto axes = make_shared<op::Constant>(element::Type_t::i32, Shape{}, 1);
    auto f =
        make_shared<Function>(make_shared<op::v1::ReduceMax>(A, axes, true), ParameterVector{A});

    auto backend = runtime::Backend::create("${BACKEND_NAME}", true);

    // Create some tensors for input/output
    Shape shape_a{3, 2};
    auto a = backend->create_tensor(element::Type_t::f32, shape_a);
    copy_data(a, vector<float>{1, 2, 3, 4, 5, 6});
    auto result = backend->create_dynamic_tensor(element::Type_t::f32, PartialShape::dynamic());

    auto handle = backend->compile(f);
    handle->call_with_validate({result}, {a});
    EXPECT_TRUE(test::all_close_f((vector<float>{2, 4, 6}), read_vector<float>(result)));
}
