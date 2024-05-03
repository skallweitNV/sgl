import test_primitive_types;

// interface ITest {
//     associatedtype Type;
//     property StructuredBuffer<Type> sb_srv;
//     property RWStructuredBuffer<Type> sb_uav;
//     property ByteAddressBuffer bab_srv;
//     property RWByteAddressBuffer bab_uav;
// };

// struct Test<T> : ITest {
//     typedef T Type;

//     StructuredBuffer<Type> _sb_srv;
//     property StructuredBuffer<Type> sb_srv { get { return _sb_srv; } }

//     RWStructuredBuffer<Type> _sb_uav;
//     property RWStructuredBuffer<Type> sb_uav { get { return _sb_uav; } }

//     ByteAddressBuffer _bab_srv;
//     property ByteAddressBuffer bab_srv { get { return _bab_srv; } }

//     RWByteAddressBuffer _bab_uav;
//     property RWByteAddressBuffer bab_uav { get { return _bab_uav; } }
// }

// struct Test_uint : Test<uint> {
//     property StructuredBuffer<Type> sb_srv { get { return _sb_srv; } }
//     property RWStructuredBuffer<Type> sb_uav { get { return _sb_uav; } }
//     property ByteAddressBuffer bab_srv { get { return _bab_srv; } }
//     property RWByteAddressBuffer bab_uav { get { return _bab_uav; } }
// };

// struct TEST : ITest {
//     typedef uint Type;

//     StructuredBuffer<Type> _sb_srv;
//     property StructuredBuffer<Type> sb_srv { get { return _sb_srv; } }

//     RWStructuredBuffer<Type> _sb_uav;
//     property RWStructuredBuffer<Type> sb_uav { get { return _sb_uav; } }

//     ByteAddressBuffer _bab_srv;
//     property ByteAddressBuffer bab_srv { get { return _bab_srv; } }

//     RWByteAddressBuffer _bab_uav;
//     property RWByteAddressBuffer bab_uav { get { return _bab_uav; } }
// }


void run_test(ITest test)
{
    typedef ITest.Type T;
    // let value1 = test.sb_srv[0];
    // test.sb_uav[0] = value1;
    let value2 = test.bab_srv.Load<T>(0);
    test.bab_uav.Store(0, value2);
    // uint tmp[sizeof(T) >> 2];
}

// typedef Test_uint TEST;

extern struct TEST : ITest;
// export struct TEST : ITest = Test_uint;

[shader("compute")]
[numthreads(1, 1, 1)]
void main(uint tid: SV_DispatchThreadID, uniform TEST test)
{
    run_test(test);
}

void run_test2<T>(
    StructuredBuffer<T> sb_srv,
    RWStructuredBuffer<T> sb_uav,
    ByteAddressBuffer bab_srv,
    RWByteAddressBuffer bab_uav
)
{
    let value1 = sb_srv[0];
    sb_uav[0] = value1;
    let value2 = bab_srv.Load<T>(0);
    bab_uav.Store(0, value2);
    // uint tmp[sizeof(T) >> 2];
}


#define MAKE_TEST(type)                                                                                                                                                     \
    [shader("compute")]                                                                                                                                                     \
    [numthreads(1, 1, 1)]                                                                                                                                                   \
    void main_##type(uint tid: SV_DispatchThreadID, StructuredBuffer<type> sb_srv, RWStructuredBuffer<type> sb_uav, ByteAddressBuffer bab_srv, RWByteAddressBuffer bab_uav) \
    {                                                                                                                                                                       \
        Test_##type test;                                                                                                                                                   \
        test._sb_srv = sb_srv;                                                                                                                                              \
        test._sb_uav = sb_uav;                                                                                                                                              \
        test._bab_srv = bab_srv;                                                                                                                                            \
        test._bab_uav = bab_uav;                                                                                                                                            \
        run_test(test);                                                                                                                                                     \
    }

// run_test2(sb_srv, sb_uav, bab_srv, bab_uav);

MAKE_TEST(uint)
MAKE_TEST(float)
