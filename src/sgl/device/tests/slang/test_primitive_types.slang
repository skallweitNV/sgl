
interface ITest {
    associatedtype Type;
    property StructuredBuffer<Type> sb_srv;
    property RWStructuredBuffer<Type> sb_uav;
    property ByteAddressBuffer bab_srv;
    property RWByteAddressBuffer bab_uav;
};

// struct Test<T> : ITest {
//     typedef T Type;

//     StructuredBuffer<Type> _sb_srv;
//     property StructuredBuffer<Type> sb_srv
//     {
//         get {
//             return _sb_srv;
//         }
//     }

//     RWStructuredBuffer<Type> _sb_uav;
//     property RWStructuredBuffer<Type> sb_uav
//     {
//         get {
//             return _sb_uav;
//         }
//     }

//     ByteAddressBuffer _bab_srv;
//     property ByteAddressBuffer bab_srv
//     {
//         get {
//             return _bab_srv;
//         }
//     }

//     RWByteAddressBuffer _bab_uav;
//     property RWByteAddressBuffer bab_uav
//     {
//         get {
//             return _bab_uav;
//         }
//     }

//     uint dummy;
// };

#define MAKE_TEST(type)                                                                                                \
    struct Test_##type : ITest {                                                                                       \
        typedef type Type;                                                                                             \
        StructuredBuffer<Type> _sb_srv;                                                                                \
        property StructuredBuffer<Type> sb_srv                                                                         \
        {                                                                                                              \
            get {                                                                                                      \
                return _sb_srv;                                                                                        \
            }                                                                                                          \
        }                                                                                                              \
        RWStructuredBuffer<Type> _sb_uav;                                                                              \
        property RWStructuredBuffer<Type> sb_uav                                                                       \
        {                                                                                                              \
            get {                                                                                                      \
                return _sb_uav;                                                                                        \
            }                                                                                                          \
        }                                                                                                              \
        ByteAddressBuffer _bab_srv;                                                                                    \
        property ByteAddressBuffer bab_srv                                                                             \
        {                                                                                                              \
            get {                                                                                                      \
                return _bab_srv;                                                                                       \
            }                                                                                                          \
        }                                                                                                              \
        RWByteAddressBuffer _bab_uav;                                                                                  \
        property RWByteAddressBuffer bab_uav                                                                           \
        {                                                                                                              \
            get {                                                                                                      \
                return _bab_uav;                                                                                       \
            }                                                                                                          \
        }                                                                                                              \
    };


MAKE_TEST(uint)
MAKE_TEST(float)

// typedef Test<uint> Test_uint;
// typedef Test<float> Test_float;
