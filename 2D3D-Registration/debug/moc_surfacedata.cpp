/****************************************************************************
** Meta object code from reading C++ file 'surfacedata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../surfacedata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'surfacedata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SurfaceData_t {
    QByteArrayData data[28];
    char stringdata0[386];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SurfaceData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SurfaceData_t qt_meta_stringdata_SurfaceData = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SurfaceData"
QT_MOC_LITERAL(1, 12, 17), // "LnReadingFinished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "FilteringFinished"
QT_MOC_LITERAL(4, 49, 17), // "monotonyCheckDone"
QT_MOC_LITERAL(5, 67, 14), // "QList<double>*"
QT_MOC_LITERAL(6, 82, 4), // "int*"
QT_MOC_LITERAL(7, 87, 16), // "showCurrentPhase"
QT_MOC_LITERAL(8, 104, 11), // "CreateChart"
QT_MOC_LITERAL(9, 116, 8), // "readOnly"
QT_MOC_LITERAL(10, 125, 15), // "readHeaderLines"
QT_MOC_LITERAL(11, 141, 6), // "QFile*"
QT_MOC_LITERAL(12, 148, 16), // "phaseRecognition"
QT_MOC_LITERAL(13, 165, 7), // "double*"
QT_MOC_LITERAL(14, 173, 5), // "bool*"
QT_MOC_LITERAL(15, 179, 14), // "newMaxDetected"
QT_MOC_LITERAL(16, 194, 19), // "checkBreathCycleDur"
QT_MOC_LITERAL(17, 214, 15), // "setFilterParams"
QT_MOC_LITERAL(18, 230, 18), // "filterNewDatapoint"
QT_MOC_LITERAL(19, 249, 15), // "extremum_search"
QT_MOC_LITERAL(20, 265, 11), // "QList<int>*"
QT_MOC_LITERAL(21, 277, 8), // "ReadIter"
QT_MOC_LITERAL(22, 286, 12), // "ReadFiltIter"
QT_MOC_LITERAL(23, 299, 22), // "ReadFiltPhaseDebugIter"
QT_MOC_LITERAL(24, 322, 24), // "ReadFiltPhasePerformIter"
QT_MOC_LITERAL(25, 347, 15), // "saveValsToLists"
QT_MOC_LITERAL(26, 363, 12), // "QStringList*"
QT_MOC_LITERAL(27, 376, 9) // "readOneLn"

    },
    "SurfaceData\0LnReadingFinished\0\0"
    "FilteringFinished\0monotonyCheckDone\0"
    "QList<double>*\0int*\0showCurrentPhase\0"
    "CreateChart\0readOnly\0readHeaderLines\0"
    "QFile*\0phaseRecognition\0double*\0bool*\0"
    "newMaxDetected\0checkBreathCycleDur\0"
    "setFilterParams\0filterNewDatapoint\0"
    "extremum_search\0QList<int>*\0ReadIter\0"
    "ReadFiltIter\0ReadFiltPhaseDebugIter\0"
    "ReadFiltPhasePerformIter\0saveValsToLists\0"
    "QStringList*\0readOneLn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SurfaceData[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  109,    2, 0x06 /* Public */,
       3,    2,  114,    2, 0x06 /* Public */,
       4,    5,  119,    2, 0x06 /* Public */,
       7,    1,  130,    2, 0x06 /* Public */,
       8,    3,  133,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  140,    2, 0x0a /* Public */,
      10,    1,  141,    2, 0x0a /* Public */,
      12,    8,  144,    2, 0x0a /* Public */,
      15,    4,  161,    2, 0x0a /* Public */,
      16,    4,  170,    2, 0x0a /* Public */,
      17,    0,  179,    2, 0x0a /* Public */,
      18,    3,  180,    2, 0x0a /* Public */,
      19,    4,  187,    2, 0x0a /* Public */,
      21,    0,  196,    2, 0x0a /* Public */,
      22,    0,  197,    2, 0x0a /* Public */,
      23,    0,  198,    2, 0x0a /* Public */,
      24,    0,  199,    2, 0x0a /* Public */,
      25,    3,  200,    2, 0x08 /* Private */,
      27,    1,  207,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 6,    2,    2,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 6, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 14,    2,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Bool, 0x80000000 | 5, 0x80000000 | 14, 0x80000000 | 6, 0x80000000 | 6,    2,    2,    2,    2,
    QMetaType::Bool, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 13, 0x80000000 | 14,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 13,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 20, 0x80000000 | 20, 0x80000000 | 14,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 26,    2,    2,    2,
    QMetaType::QStringList, 0x80000000 | 11,    2,

       0        // eod
};

void SurfaceData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SurfaceData *_t = static_cast<SurfaceData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LnReadingFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->FilteringFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->monotonyCheckDone((*reinterpret_cast< QList<double>*(*)>(_a[1])),(*reinterpret_cast< QList<double>*(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int*(*)>(_a[4])),(*reinterpret_cast< int*(*)>(_a[5]))); break;
        case 3: _t->showCurrentPhase((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 4: _t->CreateChart((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 5: _t->readOnly(); break;
        case 6: { bool _r = _t->readHeaderLines((*reinterpret_cast< QFile*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->phaseRecognition((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2])),(*reinterpret_cast< double*(*)>(_a[3])),(*reinterpret_cast< int*(*)>(_a[4])),(*reinterpret_cast< double*(*)>(_a[5])),(*reinterpret_cast< double*(*)>(_a[6])),(*reinterpret_cast< double*(*)>(_a[7])),(*reinterpret_cast< bool*(*)>(_a[8]))); break;
        case 8: { bool _r = _t->newMaxDetected((*reinterpret_cast< QList<double>*(*)>(_a[1])),(*reinterpret_cast< bool*(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->checkBreathCycleDur((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2])),(*reinterpret_cast< double*(*)>(_a[3])),(*reinterpret_cast< bool*(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: _t->setFilterParams(); break;
        case 11: _t->filterNewDatapoint((*reinterpret_cast< QList<double>*(*)>(_a[1])),(*reinterpret_cast< QList<double>*(*)>(_a[2])),(*reinterpret_cast< double*(*)>(_a[3]))); break;
        case 12: _t->extremum_search((*reinterpret_cast< QList<double>*(*)>(_a[1])),(*reinterpret_cast< QList<int>*(*)>(_a[2])),(*reinterpret_cast< QList<int>*(*)>(_a[3])),(*reinterpret_cast< bool*(*)>(_a[4]))); break;
        case 13: _t->ReadIter(); break;
        case 14: _t->ReadFiltIter(); break;
        case 15: _t->ReadFiltPhaseDebugIter(); break;
        case 16: _t->ReadFiltPhasePerformIter(); break;
        case 17: { bool _r = _t->saveValsToLists((*reinterpret_cast< QList<double>*(*)>(_a[1])),(*reinterpret_cast< QList<double>*(*)>(_a[2])),(*reinterpret_cast< QStringList*(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { QStringList _r = _t->readOneLn((*reinterpret_cast< QFile*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QFile* >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QFile* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SurfaceData::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SurfaceData::LnReadingFinished)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SurfaceData::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SurfaceData::FilteringFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SurfaceData::*_t)(QList<double> * , QList<double> * , int * , int * , int * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SurfaceData::monotonyCheckDone)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SurfaceData::*_t)(int * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SurfaceData::showCurrentPhase)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SurfaceData::*_t)(double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SurfaceData::CreateChart)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject SurfaceData::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SurfaceData.data,
      qt_meta_data_SurfaceData,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SurfaceData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SurfaceData::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SurfaceData.stringdata0))
        return static_cast<void*>(const_cast< SurfaceData*>(this));
    return QThread::qt_metacast(_clname);
}

int SurfaceData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void SurfaceData::LnReadingFinished(double _t1, double _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SurfaceData::FilteringFinished(double _t1, double _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SurfaceData::monotonyCheckDone(QList<double> * _t1, QList<double> * _t2, int * _t3, int * _t4, int * _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SurfaceData::showCurrentPhase(int * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SurfaceData::CreateChart(double _t1, double _t2, double _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
