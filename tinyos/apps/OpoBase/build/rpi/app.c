#define nx_struct struct
#define nx_union union
# 150 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/include/stddef.h" 3
typedef int ptrdiff_t;
#line 213
typedef unsigned int size_t;
#line 325
typedef unsigned int wchar_t;
# 8 "/usr/local/lib/ncc/deputy_nodeputy.h"
struct __nesc_attr_nonnull {
#line 8
  int dummy;
}  ;
#line 9
struct __nesc_attr_bnd {
#line 9
  void *lo, *hi;
}  ;
#line 10
struct __nesc_attr_bnd_nok {
#line 10
  void *lo, *hi;
}  ;
#line 11
struct __nesc_attr_count {
#line 11
  int n;
}  ;
#line 12
struct __nesc_attr_count_nok {
#line 12
  int n;
}  ;
#line 13
struct __nesc_attr_one {
#line 13
  int dummy;
}  ;
#line 14
struct __nesc_attr_one_nok {
#line 14
  int dummy;
}  ;
#line 15
struct __nesc_attr_dmemset {
#line 15
  int a1, a2, a3;
}  ;
#line 16
struct __nesc_attr_dmemcpy {
#line 16
  int a1, a2, a3;
}  ;
#line 17
struct __nesc_attr_nts {
#line 17
  int dummy;
}  ;
# 36 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/stdint.h" 3
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;



__extension__ 
typedef long long int int64_t;




typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef unsigned int uint32_t;





__extension__ 
typedef unsigned long long int uint64_t;






typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;



__extension__ 
typedef long long int int_least64_t;



typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;



__extension__ 
typedef unsigned long long int uint_least64_t;






typedef signed char int_fast8_t;





typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__ 
typedef long long int int_fast64_t;



typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__ 
typedef unsigned long long int uint_fast64_t;
#line 125
typedef int intptr_t;


typedef unsigned int uintptr_t;








__extension__ 
typedef long long int intmax_t;
__extension__ 
typedef unsigned long long int uintmax_t;
# 34 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/inttypes.h" 3
typedef unsigned int __gwchar_t;
#line 291
#line 287
typedef struct __nesc_unnamed4242 {

  long long int quot;
  long long int rem;
} imaxdiv_t;
# 281 "/usr/local/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_ntoh_uint8(const void * source)  ;










static __inline uint8_t __nesc_ntoh_leuint8(const void * source)  ;










static __inline int8_t __nesc_ntoh_int8(const void * source)  ;






static __inline uint16_t __nesc_ntoh_uint16(const void * source)  ;
#line 322
static __inline uint16_t __nesc_ntoh_leuint16(const void * source)  ;
#line 340
static __inline uint32_t __nesc_ntoh_uint32(const void * source)  ;
#line 431
typedef struct { unsigned char nxdata[1]; } __attribute__((packed)) nx_int8_t;typedef int8_t __nesc_nxbase_nx_int8_t  ;
typedef struct { unsigned char nxdata[2]; } __attribute__((packed)) nx_int16_t;typedef int16_t __nesc_nxbase_nx_int16_t  ;
typedef struct { unsigned char nxdata[4]; } __attribute__((packed)) nx_int32_t;typedef int32_t __nesc_nxbase_nx_int32_t  ;
typedef struct { unsigned char nxdata[8]; } __attribute__((packed)) nx_int64_t;typedef int64_t __nesc_nxbase_nx_int64_t  ;
typedef struct { unsigned char nxdata[1]; } __attribute__((packed)) nx_uint8_t;typedef uint8_t __nesc_nxbase_nx_uint8_t  ;
typedef struct { unsigned char nxdata[2]; } __attribute__((packed)) nx_uint16_t;typedef uint16_t __nesc_nxbase_nx_uint16_t  ;
typedef struct { unsigned char nxdata[4]; } __attribute__((packed)) nx_uint32_t;typedef uint32_t __nesc_nxbase_nx_uint32_t  ;
typedef struct { unsigned char nxdata[8]; } __attribute__((packed)) nx_uint64_t;typedef uint64_t __nesc_nxbase_nx_uint64_t  ;


typedef struct { unsigned char nxdata[1]; } __attribute__((packed)) nxle_int8_t;typedef int8_t __nesc_nxbase_nxle_int8_t  ;
typedef struct { unsigned char nxdata[2]; } __attribute__((packed)) nxle_int16_t;typedef int16_t __nesc_nxbase_nxle_int16_t  ;
typedef struct { unsigned char nxdata[4]; } __attribute__((packed)) nxle_int32_t;typedef int32_t __nesc_nxbase_nxle_int32_t  ;
typedef struct { unsigned char nxdata[8]; } __attribute__((packed)) nxle_int64_t;typedef int64_t __nesc_nxbase_nxle_int64_t  ;
typedef struct { unsigned char nxdata[1]; } __attribute__((packed)) nxle_uint8_t;typedef uint8_t __nesc_nxbase_nxle_uint8_t  ;
typedef struct { unsigned char nxdata[2]; } __attribute__((packed)) nxle_uint16_t;typedef uint16_t __nesc_nxbase_nxle_uint16_t  ;
typedef struct { unsigned char nxdata[4]; } __attribute__((packed)) nxle_uint32_t;typedef uint32_t __nesc_nxbase_nxle_uint32_t  ;
typedef struct { unsigned char nxdata[8]; } __attribute__((packed)) nxle_uint64_t;typedef uint64_t __nesc_nxbase_nxle_uint64_t  ;
# 42 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/string.h" 3
extern void *memcpy(void *__restrict __dest, const void *__restrict __src, 
size_t __n) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1, 2))) ;
#line 62
extern void *memset(void *__s, int __c, size_t __n) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1))) ;
#line 125
extern char *strcpy(char *__restrict __dest, const char *__restrict __src) 
__attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1, 2))) ;






extern char *strcat(char *__restrict __dest, const char *__restrict __src) 
__attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1, 2))) ;
# 39 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/xlocale.h" 3
#line 27
typedef struct __locale_struct {


  struct __locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;
# 395 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/string.h" 3
extern size_t strlen(const char *__s) 
__attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__pure__)) __attribute((__nonnull__(1))) ;
#line 409
extern char *strerror(int __errnum) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 30 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/types.h" 3
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;




__extension__ 
#line 46
typedef signed long long int __int64_t;
__extension__ 
#line 47
typedef unsigned long long int __uint64_t;







__extension__ 
#line 55
typedef long long int __quad_t;
__extension__ 
#line 56
typedef unsigned long long int __u_quad_t;
#line 133
__extension__ 
#line 133
typedef __u_quad_t __dev_t;
__extension__ 
#line 134
typedef unsigned int __uid_t;
__extension__ 
#line 135
typedef unsigned int __gid_t;
__extension__ 
#line 136
typedef unsigned long int __ino_t;
__extension__ 
#line 137
typedef __u_quad_t __ino64_t;
__extension__ 
#line 138
typedef unsigned int __mode_t;
__extension__ 
#line 139
typedef unsigned int __nlink_t;
__extension__ 
#line 140
typedef long int __off_t;
__extension__ 
#line 141
typedef __quad_t __off64_t;
__extension__ 
#line 142
typedef int __pid_t;
__extension__ 
#line 143
typedef struct __nesc_unnamed4243 {
#line 143
  int __val[2];
} 
#line 143
__fsid_t;
__extension__ 
#line 144
typedef long int __clock_t;
__extension__ 
#line 145
typedef unsigned long int __rlim_t;
__extension__ 
#line 146
typedef __u_quad_t __rlim64_t;
__extension__ 
#line 147
typedef unsigned int __id_t;
__extension__ 
#line 148
typedef long int __time_t;
__extension__ 
#line 149
typedef unsigned int __useconds_t;
__extension__ 
#line 150
typedef long int __suseconds_t;

__extension__ 
#line 152
typedef int __daddr_t;
__extension__ 
#line 153
typedef int __key_t;


__extension__ 
#line 156
typedef int __clockid_t;


__extension__ 
#line 159
typedef void *__timer_t;


__extension__ 
#line 162
typedef long int __blksize_t;




__extension__ 
#line 167
typedef long int __blkcnt_t;
__extension__ 
#line 168
typedef __quad_t __blkcnt64_t;


__extension__ 
#line 171
typedef unsigned long int __fsblkcnt_t;
__extension__ 
#line 172
typedef __u_quad_t __fsblkcnt64_t;


__extension__ 
#line 175
typedef unsigned long int __fsfilcnt_t;
__extension__ 
#line 176
typedef __u_quad_t __fsfilcnt64_t;


__extension__ 
#line 179
typedef int __fsword_t;

__extension__ 
#line 181
typedef int __ssize_t;


__extension__ 
#line 184
typedef long int __syscall_slong_t;

__extension__ 
#line 186
typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


__extension__ 
#line 195
typedef int __intptr_t;


__extension__ 
#line 198
typedef unsigned int __socklen_t;
# 66 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/waitstatus.h" 3
union wait {

  int w_status;
  struct __nesc_unnamed4244 {


    unsigned int __w_termsig : 7;
    unsigned int __w_coredump : 1;
    unsigned int __w_retcode : 8;
    unsigned int  : 16;
  } 






  __wait_terminated;
  struct __nesc_unnamed4245 {


    unsigned int __w_stopval : 8;
    unsigned int __w_stopsig : 8;
    unsigned int  : 16;
  } 





  __wait_stopped;
};
# 71 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/stdlib.h" 3
#line 67
typedef union __nesc_unnamed4246 {

  union wait *__uptr;
  int *__iptr;
} __WAIT_STATUS __attribute((__transparent_union__)) ;
#line 101
#line 97
typedef struct __nesc_unnamed4247 {

  int quot;
  int rem;
} div_t;







#line 105
typedef struct __nesc_unnamed4248 {

  long int quot;
  long int rem;
} ldiv_t;







__extension__ 



#line 117
typedef struct __nesc_unnamed4249 {

  long long int quot;
  long long int rem;
} lldiv_t;
#line 157
__extension__ 
#line 194
__extension__ 




__extension__ 








__extension__ 




__extension__ 
# 33 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/types.h" 3
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;



typedef __ino_t ino_t;
#line 60
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;





typedef __off_t off_t;
#line 98
typedef __pid_t pid_t;





typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 59 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/time.h" 3
typedef __clock_t clock_t;
#line 75
typedef __time_t time_t;
#line 91
typedef __clockid_t clockid_t;
#line 103
typedef __timer_t timer_t;
# 150 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/types.h" 3
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
#line 200
typedef unsigned int u_int8_t __attribute((__mode__(__QI__))) ;
typedef unsigned int u_int16_t __attribute((__mode__(__HI__))) ;
typedef unsigned int u_int32_t __attribute((__mode__(__SI__))) ;
typedef unsigned int u_int64_t __attribute((__mode__(__DI__))) ;

typedef int register_t __attribute((__mode__(__word__))) ;
# 23 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/sigset.h" 3
typedef int __sig_atomic_t;







#line 28
typedef struct __nesc_unnamed4250 {

  unsigned long int __val[1024 / (8 * sizeof(unsigned long int ))];
} __sigset_t;
# 37 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/select.h" 3
typedef __sigset_t sigset_t;
# 120 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/time.h" 3
struct timespec {

  __time_t tv_sec;
  __syscall_slong_t tv_nsec;
};
# 30 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/time.h" 3
struct timeval {

  __time_t tv_sec;
  __suseconds_t tv_usec;
};
# 48 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/select.h" 3
typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
#line 75
#line 64
typedef struct __nesc_unnamed4251 {







  __fd_mask __fds_bits[1024 / (8 * (int )sizeof(__fd_mask ))];
} 

fd_set;






typedef __fd_mask fd_mask;
#line 106
extern int select(int __nfds, fd_set *__restrict __readfds, 
fd_set *__restrict __writefds, 
fd_set *__restrict __exceptfds, 
struct timeval *__restrict __timeout);
# 31 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/sysmacros.h" 3
__extension__ 


__extension__ 


__extension__ 
# 228 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/types.h" 3
typedef __blksize_t blksize_t;






typedef __blkcnt_t blkcnt_t;



typedef __fsblkcnt_t fsblkcnt_t;



typedef __fsfilcnt_t fsfilcnt_t;
# 37 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/pthreadtypes.h" 3
typedef unsigned long int pthread_t;


union pthread_attr_t {

  char __size[36];
  long int __align;
};

typedef union pthread_attr_t pthread_attr_t;







#line 51
typedef struct __pthread_internal_slist {

  struct __pthread_internal_slist *__next;
} __pthread_slist_t;
#line 78
#line 59
typedef union __nesc_unnamed4252 {

  struct __pthread_mutex_s {

    int __lock;
    unsigned int __count;
    int __owner;


    int __kind;
    unsigned int __nusers;
    __extension__ union  {

      int __spins;
      __pthread_slist_t __list;
    } ;
  } __data;
  char __size[24];
  long int __align;
} pthread_mutex_t;





#line 80
typedef union __nesc_unnamed4253 {

  char __size[4];
  long int __align;
} pthread_mutexattr_t;
#line 104
#line 89
typedef union __nesc_unnamed4254 {

  struct __nesc_unnamed4255 {

    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;





#line 106
typedef union __nesc_unnamed4256 {

  char __size[4];
  long int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;
#line 153
#line 124
typedef union __nesc_unnamed4257 {

  struct __nesc_unnamed4258 {

    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;










    unsigned char __flags;
    unsigned char __shared;
    unsigned char __pad1;
    unsigned char __pad2;

    int __writer;
  } __data;
  char __size[32];
  long int __align;
} pthread_rwlock_t;





#line 155
typedef union __nesc_unnamed4259 {

  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;








#line 170
typedef union __nesc_unnamed4260 {

  char __size[20];
  long int __align;
} pthread_barrier_t;





#line 176
typedef union __nesc_unnamed4261 {

  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 343 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/stdlib.h" 3
struct random_data {

  int32_t *fptr;
  int32_t *rptr;
  int32_t *state;
  int rand_type;
  int rand_deg;
  int rand_sep;
  int32_t *end_ptr;
};
#line 412
struct drand48_data {

  unsigned short int __x[3];
  unsigned short int __old_x[3];
  unsigned short int __c;
  unsigned short int __init;
  unsigned long long int __a;
};
#line 542
extern void exit(int __status) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__noreturn__)) ;
#line 741
typedef int (*__compar_fn_t)(const void *arg_0x2ab95b539cf8, const void *arg_0x2ab95b537020);
#line 775
__extension__ 
#line 792
__extension__ 
# 27 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/mathdef.h" 3
typedef float float_t;

typedef double double_t;
# 193 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/math.h" 3
enum __nesc_unnamed4262 {

  FP_NAN = 

  0, 
  FP_INFINITE = 

  1, 
  FP_ZERO = 

  2, 
  FP_SUBNORMAL = 

  3, 
  FP_NORMAL = 

  4
};
#line 298
#line 291
typedef enum __nesc_unnamed4263 {

  _IEEE_ = -1, 
  _SVID_, 
  _XOPEN_, 
  _POSIX_, 
  _ISOC_
} _LIB_VERSION_TYPE;
#line 316
struct exception {


  int type;
  char *name;
  double arg1;
  double arg2;
  double retval;
};
# 47 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/ctype.h" 3
enum __nesc_unnamed4264 {

  _ISupper = 0 < 8 ? (1 << 0) << 8 : (1 << 0) >> 8, 
  _ISlower = 1 < 8 ? (1 << 1) << 8 : (1 << 1) >> 8, 
  _ISalpha = 2 < 8 ? (1 << 2) << 8 : (1 << 2) >> 8, 
  _ISdigit = 3 < 8 ? (1 << 3) << 8 : (1 << 3) >> 8, 
  _ISxdigit = 4 < 8 ? (1 << 4) << 8 : (1 << 4) >> 8, 
  _ISspace = 5 < 8 ? (1 << 5) << 8 : (1 << 5) >> 8, 
  _ISprint = 6 < 8 ? (1 << 6) << 8 : (1 << 6) >> 8, 
  _ISgraph = 7 < 8 ? (1 << 7) << 8 : (1 << 7) >> 8, 
  _ISblank = 8 < 8 ? (1 << 8) << 8 : (1 << 8) >> 8, 
  _IScntrl = 9 < 8 ? (1 << 9) << 8 : (1 << 9) >> 8, 
  _ISpunct = 10 < 8 ? (1 << 10) << 8 : (1 << 10) >> 8, 
  _ISalnum = 11 < 8 ? (1 << 11) << 8 : (1 << 11) >> 8
};
# 25 "/home/wwhuang/git/tinyos-lab11/tos/system/tos.h"
typedef uint8_t bool;
enum __nesc_unnamed4265 {
#line 26
  FALSE = 0, TRUE = 1
};
typedef nx_int8_t nx_bool;
uint16_t TOS_NODE_ID = 1;






struct __nesc_attr_atmostonce {
};
#line 37
struct __nesc_attr_atleastonce {
};
#line 38
struct __nesc_attr_exactlyonce {
};
# 51 "/home/wwhuang/git/tinyos-lab11/tos/types/TinyError.h"
enum __nesc_unnamed4266 {
  SUCCESS = 0, 
  FAIL = 1, 
  ESIZE = 2, 
  ECANCEL = 3, 
  EOFF = 4, 
  EBUSY = 5, 
  EINVAL = 6, 
  ERETRY = 7, 
  ERESERVE = 8, 
  EALREADY = 9, 
  ENOMEM = 10, 
  ENOACK = 11, 
  ELAST = 11
};

typedef uint8_t error_t  ;

static inline error_t ecombine(error_t r1, error_t r2)  ;
# 40 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/signal.h" 3
typedef __sig_atomic_t sig_atomic_t;
# 36 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/siginfo.h" 3
#line 32
typedef union sigval {

  int sival_int;
  void *sival_ptr;
} sigval_t;
#line 115
#line 50
typedef struct __nesc_unnamed4267 {

  int si_signo;
  int si_errno;

  int si_code;

  union __nesc_unnamed4268 {

    int _pad[128 / sizeof(int ) - 3];


    struct __nesc_unnamed4269 {

      __pid_t si_pid;
      __uid_t si_uid;
    } _kill;


    struct __nesc_unnamed4270 {

      int si_tid;
      int si_overrun;
      sigval_t si_sigval;
    } _timer;


    struct __nesc_unnamed4271 {

      __pid_t si_pid;
      __uid_t si_uid;
      sigval_t si_sigval;
    } _rt;


    struct __nesc_unnamed4272 {

      __pid_t si_pid;
      __uid_t si_uid;
      int si_status;
      __clock_t si_utime;
      __clock_t si_stime;
    } _sigchld;


    struct __nesc_unnamed4273 {

      void *si_addr;
    } _sigfault;


    struct __nesc_unnamed4274 {

      long int si_band;
      int si_fd;
    } _sigpoll;


    struct __nesc_unnamed4275 {

      void *_call_addr;
      int _syscall;
      unsigned int _arch;
    } _sigsys;
  } _sifields;
} siginfo_t;
#line 139
enum __nesc_unnamed4276 {

  SI_ASYNCNL = -60, 

  SI_TKILL = -6, 

  SI_SIGIO, 

  SI_ASYNCIO, 

  SI_MESGQ, 

  SI_TIMER, 

  SI_QUEUE, 

  SI_USER, 

  SI_KERNEL = 0x80
};




enum __nesc_unnamed4277 {

  ILL_ILLOPC = 1, 

  ILL_ILLOPN, 

  ILL_ILLADR, 

  ILL_ILLTRP, 

  ILL_PRVOPC, 

  ILL_PRVREG, 

  ILL_COPROC, 

  ILL_BADSTK
};



enum __nesc_unnamed4278 {

  FPE_INTDIV = 1, 

  FPE_INTOVF, 

  FPE_FLTDIV, 

  FPE_FLTOVF, 

  FPE_FLTUND, 

  FPE_FLTRES, 

  FPE_FLTINV, 

  FPE_FLTSUB
};



enum __nesc_unnamed4279 {

  SEGV_MAPERR = 1, 

  SEGV_ACCERR
};



enum __nesc_unnamed4280 {

  BUS_ADRALN = 1, 

  BUS_ADRERR, 

  BUS_OBJERR
};



enum __nesc_unnamed4281 {

  TRAP_BRKPT = 1, 

  TRAP_TRACE
};



enum __nesc_unnamed4282 {

  CLD_EXITED = 1, 

  CLD_KILLED, 

  CLD_DUMPED, 

  CLD_TRAPPED, 

  CLD_STOPPED, 

  CLD_CONTINUED
};



enum __nesc_unnamed4283 {

  POLL_IN = 1, 

  POLL_OUT, 

  POLL_MSG, 

  POLL_ERR, 

  POLL_PRI, 

  POLL_HUP
};
#line 309
#line 289
typedef struct sigevent {

  sigval_t sigev_value;
  int sigev_signo;
  int sigev_notify;

  union __nesc_unnamed4284 {

    int _pad[64 / sizeof(int ) - 3];



    __pid_t _tid;

    struct __nesc_unnamed4285 {

      void (*_function)(sigval_t arg_0x2ab95b7b6568);
      pthread_attr_t *_attribute;
    } _sigev_thread;
  } _sigev_un;
} sigevent_t;






enum __nesc_unnamed4286 {

  SIGEV_SIGNAL = 0, 

  SIGEV_NONE, 

  SIGEV_THREAD, 


  SIGEV_THREAD_ID = 4
};
# 85 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/signal.h" 3
typedef void (*__sighandler_t)(int arg_0x2ab95b7b2020);
#line 102
extern __sighandler_t signal(int __sig, __sighandler_t __handler) 
__attribute((__leaf__)) __attribute((__nothrow__)) ;
#line 217
typedef __sighandler_t sig_t;








extern int sigfillset(sigset_t *__set) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1))) ;
# 24 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/sigaction.h" 3
struct sigaction {



  union __nesc_unnamed4287 {


    __sighandler_t sa_handler;

    void (*sa_sigaction)(int arg_0x2ab95b7ca640, siginfo_t *arg_0x2ab95b7ca940, void *arg_0x2ab95b7cabe0);
  } 
  __sigaction_handler;







  __sigset_t sa_mask;


  int sa_flags;


  void (*sa_restorer)(void );
};
# 256 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/signal.h" 3
extern int sigprocmask(int __how, const sigset_t *__restrict __set, 
sigset_t *__restrict __oset) __attribute((__leaf__)) __attribute((__nothrow__)) ;
#line 315
struct sigvec {

  __sighandler_t sv_handler;
  int sv_mask;

  int sv_flags;
};
# 9 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/asm/sigcontext.h" 3
struct sigcontext {
  unsigned long trap_no;
  unsigned long error_code;
  unsigned long oldmask;
  unsigned long arm_r0;
  unsigned long arm_r1;
  unsigned long arm_r2;
  unsigned long arm_r3;
  unsigned long arm_r4;
  unsigned long arm_r5;
  unsigned long arm_r6;
  unsigned long arm_r7;
  unsigned long arm_r8;
  unsigned long arm_r9;
  unsigned long arm_r10;
  unsigned long arm_fp;
  unsigned long arm_ip;
  unsigned long arm_sp;
  unsigned long arm_lr;
  unsigned long arm_pc;
  unsigned long arm_cpsr;
  unsigned long fault_address;
};
# 25 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/sigstack.h" 3
struct sigstack {

  void *ss_sp;
  int ss_onstack;
};



enum __nesc_unnamed4288 {

  SS_ONSTACK = 1, 

  SS_DISABLE
};
#line 54
#line 49
typedef struct sigaltstack {

  void *ss_sp;
  int ss_flags;
  size_t ss_size;
} stack_t;
# 30 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/ucontext.h" 3
typedef int greg_t;





typedef greg_t gregset_t[18];


enum __nesc_unnamed4289 {

  REG_R0 = 0, 

  REG_R1 = 1, 

  REG_R2 = 2, 

  REG_R3 = 3, 

  REG_R4 = 4, 

  REG_R5 = 5, 

  REG_R6 = 6, 

  REG_R7 = 7, 

  REG_R8 = 8, 

  REG_R9 = 9, 

  REG_R10 = 10, 

  REG_R11 = 11, 

  REG_R12 = 12, 

  REG_R13 = 13, 

  REG_R14 = 14, 

  REG_R15 = 15
};


struct _libc_fpstate {

  struct __nesc_unnamed4290 {

    unsigned int sign1 : 1;
    unsigned int unused : 15;
    unsigned int sign2 : 1;
    unsigned int exponent : 14;
    unsigned int j : 1;
    unsigned int mantissa1 : 31;
    unsigned int mantissa0 : 32;
  } fpregs[8];
  unsigned int fpsr : 32;
  unsigned int fpcr : 32;
  unsigned char ftype[8];
  unsigned int init_flag;
};

typedef struct _libc_fpstate fpregset_t;





typedef struct sigcontext mcontext_t;










#line 102
typedef struct ucontext {

  unsigned long uc_flags;
  struct ucontext *uc_link;
  stack_t uc_stack;
  mcontext_t uc_mcontext;
  __sigset_t uc_sigmask;
  unsigned long uc_regspace[128] __attribute((__aligned__(8))) ;
} ucontext_t;
# 6 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/linux_atomic.h"
typedef uint8_t mcu_power_t;
typedef bool __nesc_atomic_t;



bool interupts_enabled = TRUE;
sigset_t global_sigmask;

__nesc_atomic_t __nesc_atomic_start(void );
void __nesc_atomic_end(__nesc_atomic_t reenable_interrupts);

static void __nesc_disable_interrupt(void )  ;
#line 36
static inline void __nesc_enable_interrupt(void )  ;






__nesc_atomic_t __nesc_atomic_start(void )   ;





void __nesc_atomic_end(__nesc_atomic_t reenable_interrupts)   ;
# 44 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/stdio.h" 3
struct _IO_FILE;



typedef struct _IO_FILE FILE;
#line 64
typedef struct _IO_FILE __FILE;
# 94 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/wchar.h" 3
#line 82
typedef struct __nesc_unnamed4291 {

  int __count;
  union __nesc_unnamed4292 {


    unsigned int __wch;



    char __wchb[4];
  } __value;
} __mbstate_t;
# 25 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/_G_config.h" 3
#line 21
typedef struct __nesc_unnamed4293 {

  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;




#line 26
typedef struct __nesc_unnamed4294 {

  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
# 40 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/include/stdarg.h" 3
typedef __builtin_va_list __gnuc_va_list;
# 145 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/libio.h" 3
struct _IO_jump_t;
#line 145
struct _IO_FILE;









typedef void _IO_lock_t;





struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;



  int _pos;
};
#line 181
enum __codecvt_result {

  __codecvt_ok, 
  __codecvt_partial, 
  __codecvt_error, 
  __codecvt_noconv
};
#line 246
struct _IO_FILE {
  int _flags;




  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;

  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;



  int _flags2;

  __off_t _old_offset;



  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];



  _IO_lock_t *_lock;








  __off64_t _offset;








  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;
  size_t __pad5;

  int _mode;

  char _unused2[15 * sizeof(int ) - 4 * sizeof(void *) - sizeof(size_t )];
};



typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

struct _IO_FILE_plus;
struct _IO_FILE_plus;
struct _IO_FILE_plus;
#line 339
typedef __ssize_t __io_read_fn(void *__cookie, char *__buf, size_t __nbytes);







typedef __ssize_t __io_write_fn(void *__cookie, const char *__buf, 
size_t __n);







typedef int __io_seek_fn(void *__cookie, __off64_t *__pos, int __w);


typedef int __io_close_fn(void *__cookie);
# 79 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/stdio.h" 3
typedef __gnuc_va_list va_list;
#line 110
typedef _G_fpos_t fpos_t;
#line 168
struct _IO_FILE;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
#line 336
extern int setvbuf(FILE *__restrict __stream, char *__restrict __buf, 
int __modes, size_t __n) __attribute((__leaf__)) __attribute((__nothrow__)) ;
#line 356
extern int fprintf(FILE *__restrict __stream, 
const char *__restrict __format, ...);




extern int printf(const char *__restrict __format, ...);
#line 913
extern void flockfile(FILE *__stream) __attribute((__leaf__)) __attribute((__nothrow__)) ;






extern void funlockfile(FILE *__stream) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 56 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/time.h" 3
struct timezone {

  int tz_minuteswest;
  int tz_dsttime;
};

typedef struct timezone *__restrict __timezone_ptr_t;









extern int gettimeofday(struct timeval *__restrict __tv, 
__timezone_ptr_t __tz) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((__nonnull__(1))) ;
#line 92
enum __itimer_which {


  ITIMER_REAL = 0, 


  ITIMER_VIRTUAL = 1, 



  ITIMER_PROF = 2
};




struct itimerval {


  struct timeval it_interval;

  struct timeval it_value;
};






typedef int __itimer_which_t;
# 255 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/unistd.h" 3
typedef __useconds_t useconds_t;
#line 274
typedef __socklen_t socklen_t;
#line 353
extern int close(int __fd);






extern ssize_t read(int __fd, void *__buf, size_t __nbytes);





extern ssize_t write(int __fd, const void *__buf, size_t __n);
#line 417
extern int pipe(int __pipedes[2]) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 25 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/confname.h" 3
enum __nesc_unnamed4295 {

  _PC_LINK_MAX, 

  _PC_MAX_CANON, 

  _PC_MAX_INPUT, 

  _PC_NAME_MAX, 

  _PC_PATH_MAX, 

  _PC_PIPE_BUF, 

  _PC_CHOWN_RESTRICTED, 

  _PC_NO_TRUNC, 

  _PC_VDISABLE, 

  _PC_SYNC_IO, 

  _PC_ASYNC_IO, 

  _PC_PRIO_IO, 

  _PC_SOCK_MAXBUF, 

  _PC_FILESIZEBITS, 

  _PC_REC_INCR_XFER_SIZE, 

  _PC_REC_MAX_XFER_SIZE, 

  _PC_REC_MIN_XFER_SIZE, 

  _PC_REC_XFER_ALIGN, 

  _PC_ALLOC_SIZE_MIN, 

  _PC_SYMLINK_MAX, 

  _PC_2_SYMLINKS
};



enum __nesc_unnamed4296 {

  _SC_ARG_MAX, 

  _SC_CHILD_MAX, 

  _SC_CLK_TCK, 

  _SC_NGROUPS_MAX, 

  _SC_OPEN_MAX, 

  _SC_STREAM_MAX, 

  _SC_TZNAME_MAX, 

  _SC_JOB_CONTROL, 

  _SC_SAVED_IDS, 

  _SC_REALTIME_SIGNALS, 

  _SC_PRIORITY_SCHEDULING, 

  _SC_TIMERS, 

  _SC_ASYNCHRONOUS_IO, 

  _SC_PRIORITIZED_IO, 

  _SC_SYNCHRONIZED_IO, 

  _SC_FSYNC, 

  _SC_MAPPED_FILES, 

  _SC_MEMLOCK, 

  _SC_MEMLOCK_RANGE, 

  _SC_MEMORY_PROTECTION, 

  _SC_MESSAGE_PASSING, 

  _SC_SEMAPHORES, 

  _SC_SHARED_MEMORY_OBJECTS, 

  _SC_AIO_LISTIO_MAX, 

  _SC_AIO_MAX, 

  _SC_AIO_PRIO_DELTA_MAX, 

  _SC_DELAYTIMER_MAX, 

  _SC_MQ_OPEN_MAX, 

  _SC_MQ_PRIO_MAX, 

  _SC_VERSION, 

  _SC_PAGESIZE, 


  _SC_RTSIG_MAX, 

  _SC_SEM_NSEMS_MAX, 

  _SC_SEM_VALUE_MAX, 

  _SC_SIGQUEUE_MAX, 

  _SC_TIMER_MAX, 




  _SC_BC_BASE_MAX, 

  _SC_BC_DIM_MAX, 

  _SC_BC_SCALE_MAX, 

  _SC_BC_STRING_MAX, 

  _SC_COLL_WEIGHTS_MAX, 

  _SC_EQUIV_CLASS_MAX, 

  _SC_EXPR_NEST_MAX, 

  _SC_LINE_MAX, 

  _SC_RE_DUP_MAX, 

  _SC_CHARCLASS_NAME_MAX, 


  _SC_2_VERSION, 

  _SC_2_C_BIND, 

  _SC_2_C_DEV, 

  _SC_2_FORT_DEV, 

  _SC_2_FORT_RUN, 

  _SC_2_SW_DEV, 

  _SC_2_LOCALEDEF, 


  _SC_PII, 

  _SC_PII_XTI, 

  _SC_PII_SOCKET, 

  _SC_PII_INTERNET, 

  _SC_PII_OSI, 

  _SC_POLL, 

  _SC_SELECT, 

  _SC_UIO_MAXIOV, 

  _SC_IOV_MAX = _SC_UIO_MAXIOV, 

  _SC_PII_INTERNET_STREAM, 

  _SC_PII_INTERNET_DGRAM, 

  _SC_PII_OSI_COTS, 

  _SC_PII_OSI_CLTS, 

  _SC_PII_OSI_M, 

  _SC_T_IOV_MAX, 



  _SC_THREADS, 

  _SC_THREAD_SAFE_FUNCTIONS, 

  _SC_GETGR_R_SIZE_MAX, 

  _SC_GETPW_R_SIZE_MAX, 

  _SC_LOGIN_NAME_MAX, 

  _SC_TTY_NAME_MAX, 

  _SC_THREAD_DESTRUCTOR_ITERATIONS, 

  _SC_THREAD_KEYS_MAX, 

  _SC_THREAD_STACK_MIN, 

  _SC_THREAD_THREADS_MAX, 

  _SC_THREAD_ATTR_STACKADDR, 

  _SC_THREAD_ATTR_STACKSIZE, 

  _SC_THREAD_PRIORITY_SCHEDULING, 

  _SC_THREAD_PRIO_INHERIT, 

  _SC_THREAD_PRIO_PROTECT, 

  _SC_THREAD_PROCESS_SHARED, 


  _SC_NPROCESSORS_CONF, 

  _SC_NPROCESSORS_ONLN, 

  _SC_PHYS_PAGES, 

  _SC_AVPHYS_PAGES, 

  _SC_ATEXIT_MAX, 

  _SC_PASS_MAX, 


  _SC_XOPEN_VERSION, 

  _SC_XOPEN_XCU_VERSION, 

  _SC_XOPEN_UNIX, 

  _SC_XOPEN_CRYPT, 

  _SC_XOPEN_ENH_I18N, 

  _SC_XOPEN_SHM, 


  _SC_2_CHAR_TERM, 

  _SC_2_C_VERSION, 

  _SC_2_UPE, 


  _SC_XOPEN_XPG2, 

  _SC_XOPEN_XPG3, 

  _SC_XOPEN_XPG4, 


  _SC_CHAR_BIT, 

  _SC_CHAR_MAX, 

  _SC_CHAR_MIN, 

  _SC_INT_MAX, 

  _SC_INT_MIN, 

  _SC_LONG_BIT, 

  _SC_WORD_BIT, 

  _SC_MB_LEN_MAX, 

  _SC_NZERO, 

  _SC_SSIZE_MAX, 

  _SC_SCHAR_MAX, 

  _SC_SCHAR_MIN, 

  _SC_SHRT_MAX, 

  _SC_SHRT_MIN, 

  _SC_UCHAR_MAX, 

  _SC_UINT_MAX, 

  _SC_ULONG_MAX, 

  _SC_USHRT_MAX, 


  _SC_NL_ARGMAX, 

  _SC_NL_LANGMAX, 

  _SC_NL_MSGMAX, 

  _SC_NL_NMAX, 

  _SC_NL_SETMAX, 

  _SC_NL_TEXTMAX, 


  _SC_XBS5_ILP32_OFF32, 

  _SC_XBS5_ILP32_OFFBIG, 

  _SC_XBS5_LP64_OFF64, 

  _SC_XBS5_LPBIG_OFFBIG, 


  _SC_XOPEN_LEGACY, 

  _SC_XOPEN_REALTIME, 

  _SC_XOPEN_REALTIME_THREADS, 


  _SC_ADVISORY_INFO, 

  _SC_BARRIERS, 

  _SC_BASE, 

  _SC_C_LANG_SUPPORT, 

  _SC_C_LANG_SUPPORT_R, 

  _SC_CLOCK_SELECTION, 

  _SC_CPUTIME, 

  _SC_THREAD_CPUTIME, 

  _SC_DEVICE_IO, 

  _SC_DEVICE_SPECIFIC, 

  _SC_DEVICE_SPECIFIC_R, 

  _SC_FD_MGMT, 

  _SC_FIFO, 

  _SC_PIPE, 

  _SC_FILE_ATTRIBUTES, 

  _SC_FILE_LOCKING, 

  _SC_FILE_SYSTEM, 

  _SC_MONOTONIC_CLOCK, 

  _SC_MULTI_PROCESS, 

  _SC_SINGLE_PROCESS, 

  _SC_NETWORKING, 

  _SC_READER_WRITER_LOCKS, 

  _SC_SPIN_LOCKS, 

  _SC_REGEXP, 

  _SC_REGEX_VERSION, 

  _SC_SHELL, 

  _SC_SIGNALS, 

  _SC_SPAWN, 

  _SC_SPORADIC_SERVER, 

  _SC_THREAD_SPORADIC_SERVER, 

  _SC_SYSTEM_DATABASE, 

  _SC_SYSTEM_DATABASE_R, 

  _SC_TIMEOUTS, 

  _SC_TYPED_MEMORY_OBJECTS, 

  _SC_USER_GROUPS, 

  _SC_USER_GROUPS_R, 

  _SC_2_PBS, 

  _SC_2_PBS_ACCOUNTING, 

  _SC_2_PBS_LOCATE, 

  _SC_2_PBS_MESSAGE, 

  _SC_2_PBS_TRACK, 

  _SC_SYMLOOP_MAX, 

  _SC_STREAMS, 

  _SC_2_PBS_CHECKPOINT, 


  _SC_V6_ILP32_OFF32, 

  _SC_V6_ILP32_OFFBIG, 

  _SC_V6_LP64_OFF64, 

  _SC_V6_LPBIG_OFFBIG, 


  _SC_HOST_NAME_MAX, 

  _SC_TRACE, 

  _SC_TRACE_EVENT_FILTER, 

  _SC_TRACE_INHERIT, 

  _SC_TRACE_LOG, 


  _SC_LEVEL1_ICACHE_SIZE, 

  _SC_LEVEL1_ICACHE_ASSOC, 

  _SC_LEVEL1_ICACHE_LINESIZE, 

  _SC_LEVEL1_DCACHE_SIZE, 

  _SC_LEVEL1_DCACHE_ASSOC, 

  _SC_LEVEL1_DCACHE_LINESIZE, 

  _SC_LEVEL2_CACHE_SIZE, 

  _SC_LEVEL2_CACHE_ASSOC, 

  _SC_LEVEL2_CACHE_LINESIZE, 

  _SC_LEVEL3_CACHE_SIZE, 

  _SC_LEVEL3_CACHE_ASSOC, 

  _SC_LEVEL3_CACHE_LINESIZE, 

  _SC_LEVEL4_CACHE_SIZE, 

  _SC_LEVEL4_CACHE_ASSOC, 

  _SC_LEVEL4_CACHE_LINESIZE, 



  _SC_IPV6 = _SC_LEVEL1_ICACHE_SIZE + 50, 

  _SC_RAW_SOCKETS, 


  _SC_V7_ILP32_OFF32, 

  _SC_V7_ILP32_OFFBIG, 

  _SC_V7_LP64_OFF64, 

  _SC_V7_LPBIG_OFFBIG, 


  _SC_SS_REPL_MAX, 


  _SC_TRACE_EVENT_NAME_MAX, 

  _SC_TRACE_NAME_MAX, 

  _SC_TRACE_SYS_MAX, 

  _SC_TRACE_USER_EVENT_MAX, 


  _SC_XOPEN_STREAMS, 


  _SC_THREAD_ROBUST_PRIO_INHERIT, 

  _SC_THREAD_ROBUST_PRIO_PROTECT
};



enum __nesc_unnamed4297 {

  _CS_PATH, 


  _CS_V6_WIDTH_RESTRICTED_ENVS, 



  _CS_GNU_LIBC_VERSION, 

  _CS_GNU_LIBPTHREAD_VERSION, 


  _CS_V5_WIDTH_RESTRICTED_ENVS, 



  _CS_V7_WIDTH_RESTRICTED_ENVS, 



  _CS_LFS_CFLAGS = 1000, 

  _CS_LFS_LDFLAGS, 

  _CS_LFS_LIBS, 

  _CS_LFS_LINTFLAGS, 

  _CS_LFS64_CFLAGS, 

  _CS_LFS64_LDFLAGS, 

  _CS_LFS64_LIBS, 

  _CS_LFS64_LINTFLAGS, 


  _CS_XBS5_ILP32_OFF32_CFLAGS = 1100, 

  _CS_XBS5_ILP32_OFF32_LDFLAGS, 

  _CS_XBS5_ILP32_OFF32_LIBS, 

  _CS_XBS5_ILP32_OFF32_LINTFLAGS, 

  _CS_XBS5_ILP32_OFFBIG_CFLAGS, 

  _CS_XBS5_ILP32_OFFBIG_LDFLAGS, 

  _CS_XBS5_ILP32_OFFBIG_LIBS, 

  _CS_XBS5_ILP32_OFFBIG_LINTFLAGS, 

  _CS_XBS5_LP64_OFF64_CFLAGS, 

  _CS_XBS5_LP64_OFF64_LDFLAGS, 

  _CS_XBS5_LP64_OFF64_LIBS, 

  _CS_XBS5_LP64_OFF64_LINTFLAGS, 

  _CS_XBS5_LPBIG_OFFBIG_CFLAGS, 

  _CS_XBS5_LPBIG_OFFBIG_LDFLAGS, 

  _CS_XBS5_LPBIG_OFFBIG_LIBS, 

  _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS, 


  _CS_POSIX_V6_ILP32_OFF32_CFLAGS, 

  _CS_POSIX_V6_ILP32_OFF32_LDFLAGS, 

  _CS_POSIX_V6_ILP32_OFF32_LIBS, 

  _CS_POSIX_V6_ILP32_OFF32_LINTFLAGS, 

  _CS_POSIX_V6_ILP32_OFFBIG_CFLAGS, 

  _CS_POSIX_V6_ILP32_OFFBIG_LDFLAGS, 

  _CS_POSIX_V6_ILP32_OFFBIG_LIBS, 

  _CS_POSIX_V6_ILP32_OFFBIG_LINTFLAGS, 

  _CS_POSIX_V6_LP64_OFF64_CFLAGS, 

  _CS_POSIX_V6_LP64_OFF64_LDFLAGS, 

  _CS_POSIX_V6_LP64_OFF64_LIBS, 

  _CS_POSIX_V6_LP64_OFF64_LINTFLAGS, 

  _CS_POSIX_V6_LPBIG_OFFBIG_CFLAGS, 

  _CS_POSIX_V6_LPBIG_OFFBIG_LDFLAGS, 

  _CS_POSIX_V6_LPBIG_OFFBIG_LIBS, 

  _CS_POSIX_V6_LPBIG_OFFBIG_LINTFLAGS, 


  _CS_POSIX_V7_ILP32_OFF32_CFLAGS, 

  _CS_POSIX_V7_ILP32_OFF32_LDFLAGS, 

  _CS_POSIX_V7_ILP32_OFF32_LIBS, 

  _CS_POSIX_V7_ILP32_OFF32_LINTFLAGS, 

  _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS, 

  _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS, 

  _CS_POSIX_V7_ILP32_OFFBIG_LIBS, 

  _CS_POSIX_V7_ILP32_OFFBIG_LINTFLAGS, 

  _CS_POSIX_V7_LP64_OFF64_CFLAGS, 

  _CS_POSIX_V7_LP64_OFF64_LDFLAGS, 

  _CS_POSIX_V7_LP64_OFF64_LIBS, 

  _CS_POSIX_V7_LP64_OFF64_LINTFLAGS, 

  _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS, 

  _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS, 

  _CS_POSIX_V7_LPBIG_OFFBIG_LIBS, 

  _CS_POSIX_V7_LPBIG_OFFBIG_LINTFLAGS, 


  _CS_V6_ENV, 

  _CS_V7_ENV
};
# 778 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/unistd.h" 3
extern __pid_t fork(void ) __attribute((__nothrow__)) ;
# 50 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/errno.h" 3
extern int *__errno_location(void ) __attribute((__leaf__)) __attribute((__nothrow__)) __attribute((const)) ;
# 17 "OpoBase.h"
#line 7
typedef nx_struct opo_rec_msg {
  nx_uint8_t valid;
  nx_uint32_t range;
  nx_uint32_t sequence;
  nx_uint8_t o_id[6];
  nx_int8_t rssi;
  nx_uint8_t tx_pwr;
  nx_uint8_t seconds;
  nx_uint8_t minutes;
  nx_uint8_t hours;
} __attribute__((packed)) opo_rec_t;




#line 19
typedef nx_struct opo_base_msg {
  opo_rec_t rec;
  nx_uint8_t m_id[6];
} __attribute__((packed)) opo_bmsg_t;





#line 24
typedef nx_struct opo_blink_base_msg {
  nx_uint8_t m_id[6];
  nx_uint8_t o_id[6];
  nx_uint32_t range;
} __attribute__((packed)) oblink_base_msg_t;
#line 40
#line 30
typedef nx_struct opo_visual_base_msg {
  nx_uint8_t rx_id[6];
  nx_uint8_t tx_id[6];
  nx_uint16_t ultrasonic_rf_dt;
  nx_uint16_t ultrasonic_wake_dt;
  nx_uint16_t ultrasonic_dt;
  nx_uint16_t full_time;
  nx_uint16_t seq;
  nx_uint16_t rx_fails;
  nx_uint16_t tx_fails;
} __attribute__((packed)) ovis_base_msg_t;








#line 42
typedef nx_struct opo_rx_base_msg {
  nx_uint8_t rx_id[6];
  nx_uint16_t t_rf;
  nx_uint16_t t_ultrasonic_wake;
  nx_uint16_t t_ultrasonic_wake_falling;
  nx_uint16_t t_ultrasonic;
  nx_uint16_t t_ultrasonic_falling;
} __attribute__((packed)) opo_rx_base_msg_t;





#line 51
typedef nx_struct opo_adc_msg {
  nx_uint16_t readings[6];
  nx_uint16_t times[6];
  nx_uint16_t rf_time;
} __attribute__((packed)) opo_adc_msg_t;

enum __nesc_unnamed4298 {
  OPO_DATA = 20, 
  OPO_BLINK = 21, 
  OPO_VISUAL_BASE = 22, 
  OPO_RX_BASE = 23, 
  OPO_ADC_BASE = 24, 
  OPO_VISUAL_STORE_BASE = 25
};
# 43 "/home/wwhuang/git/tinyos-lab11/tos/types/Leds.h"
enum __nesc_unnamed4299 {
  LEDS_LED0 = 1 << 0, 
  LEDS_LED1 = 1 << 1, 
  LEDS_LED2 = 1 << 2, 
  LEDS_LED3 = 1 << 3, 
  LEDS_LED4 = 1 << 4, 
  LEDS_LED5 = 1 << 5, 
  LEDS_LED6 = 1 << 6, 
  LEDS_LED7 = 1 << 7
};
# 5 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio.h"
enum __nesc_unnamed4300 {
  RPI_V2_GPIO_P1_03 = 2, 
  RPI_V2_GPIO_P1_05 = 3, 
  RPI_V2_GPIO_P1_07 = 4, 
  RPI_V2_GPIO_P1_08 = 14, 
  RPI_V2_GPIO_P1_10 = 15, 
  RPI_V2_GPIO_P1_11 = 17, 
  RPI_V2_GPIO_P1_12 = 18, 
  RPI_V2_GPIO_P1_13 = 27, 
  RPI_V2_GPIO_P1_15 = 22, 
  RPI_V2_GPIO_P1_16 = 23, 
  RPI_V2_GPIO_P1_18 = 24, 
  RPI_V2_GPIO_P1_19 = 10, 
  RPI_V2_GPIO_P1_21 = 9, 
  RPI_V2_GPIO_P1_22 = 25, 
  RPI_V2_GPIO_P1_23 = 11, 
  RPI_V2_GPIO_P1_24 = 8, 
  RPI_V2_GPIO_P1_26 = 7, 

  RPI_V2_GPIO_P5_03 = 28, 
  RPI_V2_GPIO_P5_04 = 29, 
  RPI_V2_GPIO_P5_05 = 30, 
  RPI_V2_GPIO_P5_06 = 31
};
# 28 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/fcntl.h" 3
struct flock {

  short int l_type;
  short int l_whence;

  __off_t l_start;
  __off_t l_len;




  __pid_t l_pid;
};
# 38 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/stat.h" 3
struct stat {

  __dev_t st_dev;
  unsigned short int __pad1;

  __ino_t st_ino;



  __mode_t st_mode;
  __nlink_t st_nlink;
  __uid_t st_uid;
  __gid_t st_gid;
  __dev_t st_rdev;
  unsigned short int __pad2;

  __off_t st_size;



  __blksize_t st_blksize;


  __blkcnt_t st_blocks;










  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
#line 87
  unsigned long int __unused4;
  unsigned long int __unused5;
};
# 146 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/fcntl.h" 3
extern int open(const char *__file, int __oflag, ...) __attribute((__nonnull__(1))) ;
# 57 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/mman.h" 3
extern void *mmap(void *__addr, size_t __len, int __prot, 
int __flags, int __fd, __off_t __offset) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/lib/printf/printf.h"
static inline void printfflush();
# 45 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/TinyosNetworkLayer.h"
#line 42
typedef nx_struct network_header_t {

  nxle_uint8_t network;
} __attribute__((packed)) network_header_t;
# 40 "/home/wwhuang/git/tinyos-lab11/tos/types/IeeeEui64.h"
enum __nesc_unnamed4301 {
#line 40
  IEEE_EUI64_LENGTH = 8
};


#line 42
typedef struct ieee_eui64 {
  uint8_t data[IEEE_EUI64_LENGTH];
} ieee_eui64_t;
# 47 "/home/wwhuang/git/tinyos-lab11/tos/types/Ieee154.h"
typedef uint16_t ieee154_panid_t;
typedef uint16_t ieee154_saddr_t;
typedef ieee_eui64_t ieee154_laddr_t;







#line 51
typedef struct __nesc_unnamed4302 {
  uint8_t ieee_mode : 2;
  union __nesc_unnamed4303 {
    ieee154_saddr_t saddr;
    ieee154_laddr_t laddr;
  } ieee_addr;
} ieee154_addr_t;



enum __nesc_unnamed4304 {
  IEEE154_BROADCAST_ADDR = 0xffff, 
  IEEE154_LINK_MTU = 127
};

struct ieee154_frame_addr {
  ieee154_addr_t ieee_src;
  ieee154_addr_t ieee_dst;
  ieee154_panid_t ieee_dstpan;
};

enum __nesc_unnamed4305 {
  IEEE154_MIN_HDR_SZ = 6
};
#line 86
enum ieee154_fcf_enums {
  IEEE154_FCF_FRAME_TYPE = 0, 
  IEEE154_FCF_SECURITY_ENABLED = 3, 
  IEEE154_FCF_FRAME_PENDING = 4, 
  IEEE154_FCF_ACK_REQ = 5, 
  IEEE154_FCF_INTRAPAN = 6, 
  IEEE154_FCF_DEST_ADDR_MODE = 10, 
  IEEE154_FCF_SRC_ADDR_MODE = 14
};

enum ieee154_fcf_type_enums {
  IEEE154_TYPE_BEACON = 0, 
  IEEE154_TYPE_DATA = 1, 
  IEEE154_TYPE_ACK = 2, 
  IEEE154_TYPE_MAC_CMD = 3, 
  IEEE154_TYPE_MASK = 7
};

enum ieee154_fcf_addr_mode_enums {
  IEEE154_ADDR_NONE = 0, 
  IEEE154_ADDR_SHORT = 2, 
  IEEE154_ADDR_EXT = 3, 
  IEEE154_ADDR_MASK = 3
};
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayer.h"
#line 40
typedef nx_struct ieee154_simple_header_t {

  nxle_uint16_t fcf;
  nxle_uint8_t dsn;
  nxle_uint16_t destpan;
  nxle_uint16_t dest;
  nxle_uint16_t src;
} __attribute__((packed)) ieee154_simple_header_t;
# 6 "/home/wwhuang/git/tinyos-lab11/tos/types/AM.h"
typedef nx_uint8_t nx_am_id_t;
typedef nx_uint8_t nx_am_group_t;
typedef nx_uint16_t nx_am_addr_t;

typedef uint8_t am_id_t;
typedef uint8_t am_group_t;
typedef uint16_t am_addr_t;

enum __nesc_unnamed4306 {
  AM_BROADCAST_ADDR = 0xffff
};









enum __nesc_unnamed4307 {
  TOS_AM_GROUP = 0x22, 
  TOS_AM_ADDRESS = 1
};
# 43 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayer.h"
#line 40
typedef nx_struct activemessage_header_t {

  nx_am_id_t type;
} __attribute__((packed)) activemessage_header_t;
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/MetadataFlagsLayer.h"
#line 38
typedef struct flags_metadata_t {


  uint8_t flags;
} flags_metadata_t;
# 41 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/LowPowerListeningLayer.h"
#line 38
typedef struct lpl_metadata_t {

  uint16_t sleepint;
} lpl_metadata_t;
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayer.h"
#line 38
typedef struct link_metadata_t {

  uint16_t maxRetries;
  uint16_t retryDelay;
} link_metadata_t;
# 52 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadio.h"
#line 50
typedef nx_struct cc2520_header_t {
  nxle_uint8_t length;
} __attribute__((packed)) cc2520_header_t;




#line 54
typedef struct cc2520_metadata_t {
  uint8_t rssi;
  uint8_t lqi;
} cc2520_metadata_t;



#line 59
typedef struct ack_metadata_t {
  uint8_t ack;
} ack_metadata_t;








#line 64
typedef nx_struct security_header_t {
  unsigned char __nesc_filler0[1];


  nx_uint32_t frameCounter;
  nx_uint8_t keyID[1];
} __attribute__((packed)) security_header_t;



#line 72
typedef struct timestamp_metadata_t {
  uint64_t timestamp_micro;
} timestamp_metadata_t;
#line 91
#line 76
typedef nx_struct cc2520packet_header_t {

  cc2520_header_t cc2520;
  ieee154_simple_header_t ieee154;






  network_header_t network;


  activemessage_header_t am;
} __attribute__((packed)) 
cc2520packet_header_t;



#line 93
typedef nx_struct cc2520packet_footer_t {
} __attribute__((packed)) 
cc2520packet_footer_t;










#line 97
typedef struct cc2520packet_metadata_t {



  link_metadata_t link;
  timestamp_metadata_t timestamp;
  flags_metadata_t flags;
  cc2520_metadata_t cc2520;
  ack_metadata_t ack;
} cc2520packet_metadata_t;

enum cc2520_security_enums {
  NO_SEC = 0, 
  CBC_MAC_4 = 1, 
  CBC_MAC_8 = 2, 
  CBC_MAC_16 = 3, 
  CTR_MODE = 4, 
  CCM_4 = 5, 
  CCM_8 = 6, 
  CCM_16 = 7
};
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/platform_message.h"
#line 8
typedef union message_header {
  cc2520packet_header_t cc2520;
} message_header_t;



#line 12
typedef union message_footer {
  cc2520packet_footer_t cc2520;
} message_footer_t;



#line 16
typedef union message_metadata {
  cc2520packet_metadata_t cc2520;
} message_metadata_t;
# 19 "/home/wwhuang/git/tinyos-lab11/tos/types/message.h"
#line 14
typedef nx_struct message_t {
  nx_uint8_t header[sizeof(message_header_t )];
  nx_uint8_t data[128];
  nx_uint8_t footer[sizeof(message_footer_t )];
  nx_uint8_t metadata[sizeof(message_metadata_t )];
} __attribute__((packed)) message_t;
# 41 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.h"
typedef struct __nesc_unnamed4308 {
#line 41
  int notUsed;
} 
#line 41
TSecond;
typedef struct __nesc_unnamed4309 {
#line 42
  int notUsed;
} 
#line 42
TMilli;
typedef struct __nesc_unnamed4310 {
#line 43
  int notUsed;
} 
#line 43
T32khz;
typedef struct __nesc_unnamed4311 {
#line 44
  int notUsed;
} 
#line 44
TMicro;
# 27 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/bits/ioctl-types.h" 3
struct winsize {

  unsigned short int ws_row;
  unsigned short int ws_col;
  unsigned short int ws_xpixel;
  unsigned short int ws_ypixel;
};


struct termio {

  unsigned short int c_iflag;
  unsigned short int c_oflag;
  unsigned short int c_cflag;
  unsigned short int c_lflag;
  unsigned char c_line;
  unsigned char c_cc[8];
};
# 41 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/ioctl.h" 3
extern int ioctl(int __fd, unsigned long int __request, ...) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 19 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/asm-generic/int-ll64.h" 3
typedef signed char __s8;
typedef unsigned char __u8;

typedef signed short __s16;
typedef unsigned short __u16;

typedef signed int __s32;
typedef unsigned int __u32;


__extension__ 
#line 29
typedef signed long long __s64;
__extension__ 
#line 30
typedef unsigned long long __u64;
# 26 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/linux/posix_types.h" 3
#line 24
typedef struct __nesc_unnamed4312 {
  unsigned long fds_bits[1024 / (8 * sizeof(long ))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int arg_0x2ab95bbcb508);


typedef int __kernel_key_t;
typedef int __kernel_mqd_t;
# 22 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/asm/posix_types.h" 3
typedef unsigned short __kernel_mode_t;


typedef unsigned short __kernel_ipc_pid_t;


typedef unsigned short __kernel_uid_t;
typedef unsigned short __kernel_gid_t;


typedef unsigned short __kernel_old_dev_t;
# 14 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/asm-generic/posix_types.h" 3
typedef long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;



typedef __kernel_ulong_t __kernel_ino_t;







typedef int __kernel_pid_t;
#line 40
typedef __kernel_long_t __kernel_suseconds_t;



typedef int __kernel_daddr_t;



typedef unsigned int __kernel_uid32_t;
typedef unsigned int __kernel_gid32_t;



typedef __kernel_uid_t __kernel_old_uid_t;
typedef __kernel_gid_t __kernel_old_gid_t;
#line 67
typedef unsigned int __kernel_size_t;
typedef int __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;










#line 78
typedef struct __nesc_unnamed4313 {
  int val[2];
} __kernel_fsid_t;





typedef __kernel_long_t __kernel_off_t;
typedef long long __kernel_loff_t;
typedef __kernel_long_t __kernel_time_t;
typedef __kernel_long_t __kernel_clock_t;
typedef int __kernel_timer_t;
typedef int __kernel_clockid_t;
typedef char *__kernel_caddr_t;
typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
# 27 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/linux/types.h" 3
typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

typedef __u16 __sum16;
typedef __u32 __wsum;
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiDriver.h"
struct cc2520_set_channel_data {
  uint8_t channel;
};

struct cc2520_set_address_data {
  uint16_t short_addr;
  uint64_t extended_addr;
  uint16_t pan_id;
};

struct cc2520_set_ack_data {
  uint32_t timeout;
};

struct cc2520_set_lpl_data {
  uint32_t window;
  uint32_t interval;
  bool enabled;
};

struct cc2520_set_csma_data {
  uint32_t min_backoff;
  uint32_t init_backoff;
  uint32_t cong_backoff;
  bool enabled;
};

struct cc2520_set_print_messages_data {
  uint8_t debug_level;
};

struct cc2520_set_txpower_data {
  uint8_t txpower;
};
# 27 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/sys/prctl.h" 3
extern int prctl(int __option, ...) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 6 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/../support/sdk/c/linux/file_helpers.h"
int make_nonblocking(int fd);
# 133 "/usr/lib/gcc-cross/arm-linux-gnueabi/4.7/../../../../arm-linux-gnueabi/include/time.h" 3
struct tm {

  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;


  long int tm_gmtoff;
  const char *tm_zone;
};
#line 161
struct itimerspec {

  struct timespec it_interval;
  struct timespec it_value;
};


struct sigevent;
#line 239
struct tm;



struct tm;





struct tm;




struct tm;
#line 342
extern int clock_gettime(clockid_t __clock_id, struct timespec *__tp) __attribute((__leaf__)) __attribute((__nothrow__)) ;
#line 363
extern int timer_create(clockid_t __clock_id, 
struct sigevent *__restrict __evp, 
timer_t *__restrict __timerid) __attribute((__leaf__)) __attribute((__nothrow__)) ;





extern int timer_settime(timer_t __timerid, int __flags, 
const struct itimerspec *__restrict __value, 
struct itimerspec *__restrict __ovalue) __attribute((__leaf__)) __attribute((__nothrow__)) ;
# 29 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/chips/ds2411/PlatformIeeeEui64.h"
enum __nesc_unnamed4314 {
  IEEE_EUI64_COMPANY_ID_0 = 0xc0, 
  IEEE_EUI64_COMPANY_ID_1 = 0x98, 
  IEEE_EUI64_COMPANY_ID_2 = 0xe5, 
  IEEE_EUI64_SERIAL_ID_0 = 'R', 
  IEEE_EUI64_SERIAL_ID_1 = 'P'
};
# 6 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/ds2411.h"
enum __nesc_unnamed4315 {
  DS2411_SERIAL_LENGTH = 6, 
  DS2411_DATA_LENGTH = 8
};








#line 11
typedef union ds241_serial_t {
  uint8_t data[DS2411_DATA_LENGTH];
  struct  {
    uint8_t family_code;
    uint8_t serial[DS2411_SERIAL_LENGTH];
    uint8_t crc;
  } ;
} ds2411_serial_t;
typedef uint8_t CC2520RpiAmRadioP__PacketRSSI__value_type;
typedef uint8_t CC2520RpiAmRadioP__PacketLinkQuality__value_type;
typedef uint8_t CC2520RpiAmPacketMetadataP__PacketRSSI__value_type;
typedef uint8_t CC2520RpiAmPacketMetadataP__PacketLinkQuality__value_type;
typedef TMicro OneWireMasterP__BusyWait__precision_tag;
typedef uint16_t OneWireMasterP__BusyWait__size_type;
typedef TMicro BusyWaitMicroP__BusyWait__precision_tag;
typedef uint16_t BusyWaitMicroP__BusyWait__size_type;
typedef TMilli /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__precision_tag;
typedef TMilli /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__precision_tag;
typedef uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__size_type;
typedef TMilli /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__precision_tag;
typedef /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__precision_tag /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__precision_tag;
typedef uint32_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type;
typedef /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__precision_tag /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__precision_tag;
typedef TMilli /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__precision_tag;
typedef /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__precision_tag /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__precision_tag;
typedef /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__precision_tag /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__precision_tag;
typedef TMilli /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__precision_tag;
typedef /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__precision_tag /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__LocalTime__precision_tag;
typedef /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__precision_tag /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__Counter__precision_tag;
typedef uint32_t /*HilTimerMilliC.CounterToLocalTimeC*/CounterToLocalTimeC__0__Counter__size_type;
typedef TMilli LocalTimeMilli32P__LocalTime__precision_tag;
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t PlatformP__Init__init(void );
#line 62
static error_t PlatformP__InterruptInit__default__init(void );
#line 62
static error_t LedsP__Init__init(void );
# 61 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Leds.nc"
static void LedsP__Leds__led0Off(void );
#line 77
static void LedsP__Leds__led1Off(void );
#line 94
static void LedsP__Leds__led2Off(void );
# 47 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void HplBcm2835GeneralIOP__Gpio__makeInput(
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
uint8_t arg_0x2ab95ba72a60);
# 42 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static bool HplBcm2835GeneralIOP__Gpio__get(
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
uint8_t arg_0x2ab95ba72a60);
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void HplBcm2835GeneralIOP__Gpio__makeOutput(
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
uint8_t arg_0x2ab95ba72a60);
# 17 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void HplBcm2835GeneralIOP__Gpio__set(
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
uint8_t arg_0x2ab95ba72a60);
# 22 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void HplBcm2835GeneralIOP__Gpio__clr(
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
uint8_t arg_0x2ab95ba72a60);
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t HplBcm2835GeneralIOP__Init__init(void );
# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__makeOutput(void );
#line 40
static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__set(void );





static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__makeOutput(void );
#line 40
static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__set(void );





static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__makeOutput(void );
#line 40
static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__set(void );
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t SchedulerBasicP__TaskBasic__postTask(
# 56 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x2ab95b77e9e0);
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void SchedulerBasicP__TaskBasic__default__runTask(
# 56 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x2ab95b77e9e0);
# 57 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Scheduler.nc"
static void SchedulerBasicP__Scheduler__init(void );
#line 72
static void SchedulerBasicP__Scheduler__taskLoop(void );
#line 65
static bool SchedulerBasicP__Scheduler__runNextTask(void );
# 76 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/McuSleep.nc"
static void McuSleepP__McuSleep__sleep(void );
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static error_t IOManagerP__IO__registerFileDescriptor(
# 12 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
uint8_t arg_0x2ab95b9ae850, 
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
int file_descriptor);




static void IOManagerP__IO__default__receiveReady(
# 12 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
uint8_t arg_0x2ab95b9ae850);
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/BlockingIO.nc"
static void IOManagerP__BlockingIO__waitForIO(void );
# 60 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Boot.nc"
static void OpoBaseP__Boot__booted(void );
# 113 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
static void OpoBaseP__RfControl__startDone(error_t error);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



OpoBaseP__OpoReceive__receive(
#line 71
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



OpoBaseP__BlinkReceive__receive(
#line 71
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



OpoBaseP__VisualReceive__receive(
#line 71
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



OpoBaseP__RxReceive__receive(
#line 71
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



OpoBaseP__ADCReceive__receive(
#line 71
message_t * msg, 
void * payload, 





uint8_t len);
# 40 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageConfig.nc"
static am_addr_t CC2520RpiAmRadioP__ActiveMessageConfig__destination(message_t *msg);
# 85 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/PacketAcknowledgements.nc"
static bool CC2520RpiAmRadioP__PacketAcknowledgements__wasAcked(
#line 80
message_t * msg);
# 104 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
static error_t CC2520RpiRadioP__SplitControl__start(void );
# 85 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
static void CC2520RpiRadioP__PacketMetadata__setWasAcked(message_t *msg, bool ack);
#line 13
static void CC2520RpiRadioP__PacketMetadata__setTimestamp(message_t *msg, uint64_t timestamp);
#line 7
static void CC2520RpiRadioP__PacketMetadata__setLqi(message_t *msg, uint8_t lqi);
static void CC2520RpiRadioP__PacketMetadata__setRssi(message_t *msg, uint8_t rssi);
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t CC2520RpiReceiveP__SoftwareInit__init(void );
# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static void CC2520RpiReceiveP__IO__receiveReady(void );
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void CC2520RpiReceiveP__receive_task__runTask(void );
# 5 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/time/UnixTime.nc"
static uint64_t UnixTimeP__UnixTime__getMicroseconds(void );
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t CC2520RpiSendP__SoftwareInit__init(void );
# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static void CC2520RpiSendP__IO__receiveReady(void );
# 46 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static error_t CC2520RpiSendP__BareSend__send(message_t *msg);
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void CC2520RpiSendP__sendDone_task__runTask(void );
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t CC2520RpiAmPacketP__RadioPacket__payloadLength(message_t *msg);
#line 43
static uint8_t CC2520RpiAmPacketP__RadioPacket__headerLength(message_t *msg);
#line 65
static uint8_t CC2520RpiAmPacketP__RadioPacket__metadataLength(message_t *msg);
# 48 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/LocalIeeeEui64.nc"
static ieee_eui64_t LocalIeeeEui64P__LocalIeeeEui64__getId(void );
# 13 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/ReadId48.nc"
static error_t Ds2411P__ReadId48__read(uint8_t *id);
# 10 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireReadWrite.nc"
static error_t OneWireMasterP__OneWire__read(uint8_t cmd, uint8_t *buf, uint8_t len);
# 66 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/BusyWait.nc"
static void BusyWaitMicroP__BusyWait__wait(BusyWaitMicroP__BusyWait__size_type dt);
# 44 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeInput(void );
#line 43
static bool /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__get(void );


static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeOutput(void );
#line 41
static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__clr(void );
# 95 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/StdControl.nc"
static error_t Ds2411PowerControlC__StdControl__start(void );









static error_t Ds2411PowerControlC__StdControl__stop(void );
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubReceive__receive(message_t *msg);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubSend__sendDone(message_t *msg, error_t error);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Packet.nc"
static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Packet__payloadLength(
#line 74
message_t * msg);
# 110 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMSend.nc"
static void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__default__sendDone(
# 45 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea73c8, 
# 103 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMSend.nc"
message_t * msg, 






error_t error);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__default__receive(
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea51a0, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__headerLength(message_t *msg);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__default__receive(
# 46 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea65f8, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__default__receive(
# 51 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea48b0, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
# 68 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMPacket.nc"
static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__address(void );









static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__destination(
#line 74
message_t * amsg);
#line 147
static am_id_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__type(
#line 143
message_t * amsg);
#line 136
static bool /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__isForMe(
#line 133
message_t * amsg);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__default__receive(
# 52 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea2488, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
# 55 "/home/wwhuang/git/tinyos-lab11/tos/system/ActiveMessageAddressC.nc"
static am_addr_t ActiveMessageAddressC__amAddress(void );
# 50 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/ActiveMessageAddress.nc"
static am_addr_t ActiveMessageAddressC__ActiveMessageAddress__amAddress(void );
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubSend__sendDone(message_t *msg, error_t error);
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubReceive__receive(message_t *msg);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__headerLength(message_t *msg);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void CC2520RpiAmUniqueP__SubSend__sendDone(message_t *msg, error_t error);
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t CC2520RpiAmUniqueP__Init__init(void );
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void CC2520RpiAm154DummyP__Ieee154Send__sendDone(message_t *msg, error_t error);
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t *CC2520RpiAm154DummyP__Ieee154Receive__receive(message_t *msg);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__headerLength(message_t *msg);
# 141 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayer.nc"
static uint16_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__Ieee154PacketLayer__getDestAddr(message_t *msg);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__sendDone(message_t *msg, error_t error);
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__runTask(void );
# 83 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__fired(void );
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__headerLength(message_t *msg);
#line 65
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__metadataLength(message_t *msg);
# 65 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/PacketLink.nc"
static uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetryDelay(
#line 62
message_t * msg);
#line 46
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__setRetries(
#line 42
message_t * msg, 



uint16_t maxRetries);
#line 59
static uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetries(
#line 56
message_t * msg);
# 109 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
static /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__size_type /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getNow(void );
#line 103
static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__startAt(/*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__size_type t0, /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__size_type dt);
#line 116
static /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__size_type /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getAlarm(void );
#line 73
static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__stop(void );
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Init__init(void );
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__runTask(void );
# 78 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__fired(void );
# 136 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static uint32_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__getNow(void );
#line 129
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__startOneShotAt(uint32_t t0, uint32_t dt);
#line 78
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__stop(void );
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__runTask(void );
# 83 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__fired(void );
#line 83
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__default__fired(
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
uint8_t arg_0x2ab95c11ae68);
# 73 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__startOneShot(
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
uint8_t arg_0x2ab95c11ae68, 
# 73 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
uint32_t dt);
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t PlatformP__InterruptInit__init(void );
# 61 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Leds.nc"
static void PlatformP__Leds__led0Off(void );
#line 77
static void PlatformP__Leds__led1Off(void );
#line 94
static void PlatformP__Leds__led2Off(void );
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t PlatformP__GpioInit__init(void );
#line 62
static error_t PlatformP__LedsInit__init(void );
# 18 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc"
static inline void PlatformP__sigint_handler(int sig);








static inline error_t PlatformP__Init__init(void );
#line 48
static inline error_t PlatformP__InterruptInit__default__init(void );
# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
static void LedsP__Led0__makeOutput(void );
#line 40
static void LedsP__Led0__set(void );





static void LedsP__Led1__makeOutput(void );
#line 40
static void LedsP__Led1__set(void );





static void LedsP__Led2__makeOutput(void );
#line 40
static void LedsP__Led2__set(void );
# 56 "/home/wwhuang/git/tinyos-lab11/tos/system/LedsP.nc"
static inline error_t LedsP__Init__init(void );
#line 79
static inline void LedsP__Leds__led0Off(void );
#line 94
static inline void LedsP__Leds__led1Off(void );
#line 109
static inline void LedsP__Leds__led2Off(void );
# 34 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
volatile unsigned *HplBcm2835GeneralIOP__gpio;



unsigned int HplBcm2835GeneralIOP__pin_level = 0;
unsigned int HplBcm2835GeneralIOP__pin_output = 0;


static inline error_t HplBcm2835GeneralIOP__Init__init(void );
#line 78
static void HplBcm2835GeneralIOP__Gpio__set(uint8_t bcm_pin);




static inline void HplBcm2835GeneralIOP__Gpio__clr(uint8_t bcm_pin);
#line 104
static bool HplBcm2835GeneralIOP__Gpio__get(uint8_t bcm_pin);



static void HplBcm2835GeneralIOP__Gpio__makeInput(uint8_t bcm_pin);








static void HplBcm2835GeneralIOP__Gpio__makeOutput(uint8_t bcm_pin);
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__makeOutput(void );
#line 17
static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__set(void );
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__set(void );





static inline void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__makeOutput(void );
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__makeOutput(void );
#line 17
static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__set(void );
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__set(void );





static inline void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__makeOutput(void );
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__makeOutput(void );
#line 17
static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__set(void );
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__set(void );





static inline void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__makeOutput(void );
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t RealMainP__SoftwareInit__init(void );
# 60 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Boot.nc"
static void RealMainP__Boot__booted(void );
# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
static error_t RealMainP__PlatformInit__init(void );
# 57 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Scheduler.nc"
static void RealMainP__Scheduler__init(void );
#line 72
static void RealMainP__Scheduler__taskLoop(void );
#line 65
static bool RealMainP__Scheduler__runNextTask(void );
# 63 "/home/wwhuang/git/tinyos-lab11/tos/system/RealMainP.nc"
int main(void )   ;
# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void SchedulerBasicP__TaskBasic__runTask(
# 56 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x2ab95b77e9e0);
# 76 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/McuSleep.nc"
static void SchedulerBasicP__McuSleep__sleep(void );
# 61 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
enum SchedulerBasicP____nesc_unnamed4316 {

  SchedulerBasicP__NUM_TASKS = 5U, 
  SchedulerBasicP__NO_TASK = 255
};

uint8_t SchedulerBasicP__m_head;
uint8_t SchedulerBasicP__m_tail;
uint8_t SchedulerBasicP__m_next[SchedulerBasicP__NUM_TASKS];








static __inline uint8_t SchedulerBasicP__popTask(void );
#line 97
static inline bool SchedulerBasicP__isWaiting(uint8_t id);




static inline bool SchedulerBasicP__pushTask(uint8_t id);
#line 124
static inline void SchedulerBasicP__Scheduler__init(void );









static bool SchedulerBasicP__Scheduler__runNextTask(void );
#line 149
static inline void SchedulerBasicP__Scheduler__taskLoop(void );
#line 170
static error_t SchedulerBasicP__TaskBasic__postTask(uint8_t id);




static void SchedulerBasicP__TaskBasic__default__runTask(uint8_t id);
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/BlockingIO.nc"
static void McuSleepP__BlockingIO__waitForIO(void );
# 20 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/McuSleepP.nc"
static inline void McuSleepP__McuSleep__sleep(void );
# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static void IOManagerP__IO__receiveReady(
# 12 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
uint8_t arg_0x2ab95b9ae850);






enum IOManagerP____nesc_unnamed4317 {
  IOManagerP__N_FDS = 2U
};

uint8_t IOManagerP__map[IOManagerP__N_FDS];
fd_set IOManagerP__rfds;
bool IOManagerP__is_init = FALSE;


static error_t IOManagerP__IO__registerFileDescriptor(uint8_t id, int file_descriptor);
#line 47
static inline void IOManagerP__BlockingIO__waitForIO(void );
#line 92
static inline void IOManagerP__IO__default__receiveReady(uint8_t id);
# 104 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
static error_t OpoBaseP__RfControl__start(void );
# 20 "OpoBaseP.nc"
static inline void OpoBaseP__Boot__booted(void );





static inline message_t *OpoBaseP__OpoReceive__receive(message_t *msg, void *payload, uint8_t len);
#line 52
static inline message_t *OpoBaseP__BlinkReceive__receive(message_t *msg, void *payload, uint8_t len);
#line 72
static inline message_t *OpoBaseP__VisualReceive__receive(message_t *msg, void *payload, uint8_t len);
#line 103
static inline message_t *OpoBaseP__RxReceive__receive(message_t *msg, void *payload, uint8_t len);
#line 124
static inline message_t *OpoBaseP__ADCReceive__receive(message_t *msg, void *payload, uint8_t len);
#line 137
static inline void OpoBaseP__RfControl__startDone(error_t err);
# 141 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayer.nc"
static uint16_t CC2520RpiAmRadioP__Ieee154PacketLayer__getDestAddr(message_t *msg);
# 110 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmRadioP.nc"
static inline bool CC2520RpiAmRadioP__PacketAcknowledgements__wasAcked(message_t *msg);
#line 219
static inline am_addr_t CC2520RpiAmRadioP__ActiveMessageConfig__destination(message_t *msg);
# 113 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
static void CC2520RpiRadioP__SplitControl__startDone(error_t error);
# 48 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/LocalIeeeEui64.nc"
static ieee_eui64_t CC2520RpiRadioP__LocalIeeeEui64__getId(void );
# 31 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
int CC2520RpiRadioP__cc2520_file = -1;
struct cc2520_set_channel_data CC2520RpiRadioP__chan_data = { 16 };
struct cc2520_set_address_data CC2520RpiRadioP__addr_data = { 0, 0, 0x22 };

struct cc2520_set_ack_data CC2520RpiRadioP__ack_data = { 5000 };
struct cc2520_set_txpower_data CC2520RpiRadioP__txpower_data = { 0x32 };





struct cc2520_set_lpl_data CC2520RpiRadioP__lpl_data = { 0, 0, FALSE };




struct cc2520_set_print_messages_data CC2520RpiRadioP__print_data = { 0 };


ieee_eui64_t CC2520RpiRadioP__ext_addr;

uint8_t CC2520RpiRadioP__sequence_number;

static inline error_t CC2520RpiRadioP__SplitControl__start(void );
#line 245
static inline cc2520_metadata_t *CC2520RpiRadioP__getMetaCC2520(message_t *msg);



static inline ack_metadata_t *CC2520RpiRadioP__getMetaAck(message_t *msg);



static inline timestamp_metadata_t *CC2520RpiRadioP__getMetaTimestamp(message_t *msg);
#line 269
static inline void CC2520RpiRadioP__PacketMetadata__setLqi(message_t *msg, uint8_t lqi);



static inline void CC2520RpiRadioP__PacketMetadata__setRssi(message_t *msg, uint8_t rssi);



static inline void CC2520RpiRadioP__PacketMetadata__setTimestamp(message_t *msg, uint64_t t);
#line 312
static inline void CC2520RpiRadioP__PacketMetadata__setWasAcked(message_t *msg, bool ack);
# 5 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/time/UnixTime.nc"
static uint64_t CC2520RpiReceiveP__UnixTime__getMicroseconds(void );
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t *CC2520RpiReceiveP__BareReceive__receive(message_t *msg);
# 13 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
static void CC2520RpiReceiveP__PacketMetadata__setTimestamp(message_t *msg, uint64_t timestamp);
#line 7
static void CC2520RpiReceiveP__PacketMetadata__setLqi(message_t *msg, uint8_t lqi);
static void CC2520RpiReceiveP__PacketMetadata__setRssi(message_t *msg, uint8_t rssi);
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static error_t CC2520RpiReceiveP__IO__registerFileDescriptor(int file_descriptor);
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t CC2520RpiReceiveP__receive_task__postTask(void );
# 51 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc"
enum CC2520RpiReceiveP____nesc_unnamed4318 {
#line 51
  CC2520RpiReceiveP__receive_task = 0U
};
#line 51
typedef int CC2520RpiReceiveP____nesc_sillytask_receive_task[CC2520RpiReceiveP__receive_task];
#line 28
int CC2520RpiReceiveP__cc2520_pipe;

uint8_t *CC2520RpiReceiveP__rx_msg_ptr;
message_t CC2520RpiReceiveP__rx_msg_buf;

int CC2520RpiReceiveP__read_pipe[2];
#line 51
static inline void CC2520RpiReceiveP__receive_task__runTask(void );
#line 101
static inline void CC2520RpiReceiveP__IO__receiveReady(void );
#line 143
static inline error_t CC2520RpiReceiveP__SoftwareInit__init(void );
# 27 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/time/UnixTimeP.nc"
static inline uint64_t UnixTimeP__UnixTime__getMicroseconds(void );
# 85 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
static void CC2520RpiSendP__PacketMetadata__setWasAcked(message_t *msg, bool ack);
# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
static error_t CC2520RpiSendP__IO__registerFileDescriptor(int file_descriptor);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void CC2520RpiSendP__BareSend__sendDone(message_t *msg, error_t error);
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t CC2520RpiSendP__sendDone_task__postTask(void );
# 53 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc"
enum CC2520RpiSendP____nesc_unnamed4319 {
#line 53
  CC2520RpiSendP__sendDone_task = 1U
};
#line 53
typedef int CC2520RpiSendP____nesc_sillytask_sendDone_task[CC2520RpiSendP__sendDone_task];
#line 21
#line 18
typedef struct CC2520RpiSendP____nesc_unnamed4320 {
  message_t *ptr_to_msg;
  uint8_t len;
} CC2520RpiSendP__write_fifo_header_t;





#line 23
typedef struct CC2520RpiSendP____nesc_unnamed4321 {
  message_t *ptr_to_msg;
  ssize_t ret;
  uint8_t len;
} CC2520RpiSendP__read_fifo_header_t;

int CC2520RpiSendP__cc2520_read;
int CC2520RpiSendP__cc2520_write;
int CC2520RpiSendP__write_pipe[2];
int CC2520RpiSendP__read_pipe[2];

CC2520RpiSendP__read_fifo_header_t CC2520RpiSendP__send_hdr;
#line 53
static inline void CC2520RpiSendP__sendDone_task__runTask(void );
#line 80
static inline error_t CC2520RpiSendP__SoftwareInit__init(void );
#line 201
static inline void CC2520RpiSendP__IO__receiveReady(void );
#line 235
static inline error_t CC2520RpiSendP__BareSend__send(message_t *msg);
# 16 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmPacketP.nc"
static inline uint8_t CC2520RpiAmPacketP__RadioPacket__headerLength(message_t *msg);






static inline uint8_t CC2520RpiAmPacketP__RadioPacket__payloadLength(message_t *msg);
#line 50
static inline uint8_t CC2520RpiAmPacketP__RadioPacket__metadataLength(message_t *msg);
# 13 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/ReadId48.nc"
static error_t LocalIeeeEui64P__ReadId48__read(uint8_t *id);
# 13 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/LocalIeeeEui64P.nc"
ieee_eui64_t LocalIeeeEui64P__eui = { { 0x00 } };

bool LocalIeeeEui64P__have_id = FALSE;

static inline ieee_eui64_t LocalIeeeEui64P__LocalIeeeEui64__getId(void );
# 10 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireReadWrite.nc"
static error_t Ds2411P__OneWire__read(uint8_t cmd, uint8_t *buf, uint8_t len);
# 95 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/StdControl.nc"
static error_t Ds2411P__PowerControl__start(void );









static error_t Ds2411P__PowerControl__stop(void );
# 19 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411P.nc"
bool Ds2411P__haveId = FALSE;
ds2411_serial_t Ds2411P__ds2411id;



static inline bool Ds2411P__ds2411_check_crc(const ds2411_serial_t *id);
#line 41
static inline error_t Ds2411P__readId(void );
#line 69
static inline error_t Ds2411P__ReadId48__read(uint8_t *id);
# 66 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/BusyWait.nc"
static void OneWireMasterP__BusyWait__wait(OneWireMasterP__BusyWait__size_type dt);
# 44 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
static void OneWireMasterP__Pin__makeInput(void );
#line 43
static bool OneWireMasterP__Pin__get(void );


static void OneWireMasterP__Pin__makeOutput(void );
#line 41
static void OneWireMasterP__Pin__clr(void );
# 27 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireMasterP.nc"
#line 20
typedef enum OneWireMasterP____nesc_unnamed4322 {
  OneWireMasterP__DELAY_5US = 5, 
  OneWireMasterP__RESET_LOW_TIME = 560, 
  OneWireMasterP__DELAY_60US = 60, 
  OneWireMasterP__PRESENCE_DETECT_LOW_TIME = 240, 
  OneWireMasterP__PRESENCE_RESET_HIGH_TIME = 480, 
  OneWireMasterP__SLOT_TIME = 65
} OneWireMasterP__onewiretimes_t;






static inline bool OneWireMasterP__reset(void );
#line 52
static inline void OneWireMasterP__writeOne(void );






static inline void OneWireMasterP__writeZero(void );






static inline bool OneWireMasterP__readBit(void );










static inline void OneWireMasterP__writeByte(uint8_t c);
#line 89
static inline uint8_t OneWireMasterP__readByte(void );










static inline error_t OneWireMasterP__OneWire__read(uint8_t cmd, uint8_t *buf, uint8_t len);
# 11 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/BusyWaitMicroP.nc"
static void BusyWaitMicroP__BusyWait__wait(uint16_t dt);
# 47 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeInput(void );
#line 42
static bool /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__get(void );
#line 54
static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeOutput(void );
#line 22
static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__clr(void );
# 16 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__clr(void );

static inline bool /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__get(void );
static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeInput(void );

static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeOutput(void );
# 19 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411PowerControlC.nc"
static inline error_t Ds2411PowerControlC__StdControl__start(void );



static inline error_t Ds2411PowerControlC__StdControl__stop(void );
# 110 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMSend.nc"
static void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__sendDone(
# 45 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea73c8, 
# 103 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMSend.nc"
message_t * msg, 






error_t error);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__receive(
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea51a0, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
# 40 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageConfig.nc"
static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Config__destination(message_t *msg);
# 50 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/ActiveMessageAddress.nc"
static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ActiveMessageAddress__amAddress(void );
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__headerLength(message_t *msg);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__receive(
# 46 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea65f8, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__receive(
# 51 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea48b0, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
#line 78
static 
#line 74
message_t * 



/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__receive(
# 52 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x2ab95bea2488, 
# 71 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
message_t * msg, 
void * payload, 





uint8_t len);
# 67 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline activemessage_header_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getHeader(message_t *msg);




static inline void */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getPayload(message_t *msg);
#line 98
static __inline void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubSend__sendDone(message_t *msg, error_t error);









static inline void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__default__sendDone(am_id_t id, message_t *msg, error_t error);
#line 128
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubReceive__receive(message_t *msg);
#line 141
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len);




static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len);




static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len);




static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len);






static __inline am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__address(void );









static __inline bool /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__isForMe(message_t *msg);





static __inline am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__destination(message_t *msg);
#line 199
static __inline am_id_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__type(message_t *msg);
#line 225
static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__headerLength(message_t *msg);




static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__payloadLength(message_t *msg);
#line 262
static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Packet__payloadLength(message_t *msg);
# 63 "/home/wwhuang/git/tinyos-lab11/tos/system/ActiveMessageAddressC.nc"
am_addr_t ActiveMessageAddressC__addr = TOS_AM_ADDRESS;









static inline am_addr_t ActiveMessageAddressC__ActiveMessageAddress__amAddress(void );
#line 120
static inline am_addr_t ActiveMessageAddressC__amAddress(void );
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosReceive__receive(message_t *msg);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__headerLength(message_t *msg);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosSend__sendDone(message_t *msg, error_t error);
#line 54
static void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Send__sendDone(message_t *msg, error_t error);
# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Receive__receive(message_t *msg);
# 127 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/TinyosNetworkLayerC.nc"
static inline network_header_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__getHeader(message_t *msg);
#line 145
enum /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0____nesc_unnamed4323 {


  TinyosNetworkLayerC__0__PAYLOAD_OFFSET = sizeof(network_header_t )
};




static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__headerLength(message_t *msg);




static inline uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__payloadLength(message_t *msg);
#line 214
static inline void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubSend__sendDone(message_t *msg, error_t result);







static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubReceive__receive(message_t *msg);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void CC2520RpiAmUniqueP__Send__sendDone(message_t *msg, error_t error);
# 16 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmUniqueP.nc"
uint8_t CC2520RpiAmUniqueP__seq_no;

static inline error_t CC2520RpiAmUniqueP__Init__init(void );
#line 33
static inline void CC2520RpiAmUniqueP__SubSend__sendDone(message_t *msg, error_t error);
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAm154DummyP.nc"
static inline message_t *CC2520RpiAm154DummyP__Ieee154Receive__receive(message_t *msg);
static inline void CC2520RpiAm154DummyP__Ieee154Send__sendDone(message_t *msg, error_t error);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__headerLength(message_t *msg);
# 57 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayerP.nc"
enum /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0____nesc_unnamed4324 {

  Ieee154PacketLayerP__0__IEEE154_DATA_FRAME_MASK = (((IEEE154_TYPE_MASK << IEEE154_FCF_FRAME_TYPE)
   | (1 << IEEE154_FCF_INTRAPAN))
   | (IEEE154_ADDR_MASK << IEEE154_FCF_DEST_ADDR_MODE))
   | (IEEE154_ADDR_MASK << IEEE154_FCF_SRC_ADDR_MODE), 

  Ieee154PacketLayerP__0__IEEE154_DATA_FRAME_VALUE = (((IEEE154_TYPE_DATA << IEEE154_FCF_FRAME_TYPE)
   | (1 << IEEE154_FCF_INTRAPAN))
   | (IEEE154_ADDR_SHORT << IEEE154_FCF_DEST_ADDR_MODE))
   | (IEEE154_ADDR_SHORT << IEEE154_FCF_SRC_ADDR_MODE), 

  Ieee154PacketLayerP__0__IEEE154_DATA_FRAME_PRESERVE = (1 << IEEE154_FCF_ACK_REQ)
   | (1 << IEEE154_FCF_FRAME_PENDING), 

  Ieee154PacketLayerP__0__IEEE154_ACK_FRAME_LENGTH = 3, 
  Ieee154PacketLayerP__0__IEEE154_ACK_FRAME_MASK = IEEE154_TYPE_MASK << IEEE154_FCF_FRAME_TYPE, 
  Ieee154PacketLayerP__0__IEEE154_ACK_FRAME_VALUE = IEEE154_TYPE_ACK << IEEE154_FCF_FRAME_TYPE
};

static inline ieee154_simple_header_t */*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__getHeader(message_t *msg);
#line 178
static inline uint16_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__Ieee154PacketLayer__getDestAddr(message_t *msg);
#line 282
static inline uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__headerLength(message_t *msg);




static inline uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__payloadLength(message_t *msg);
# 46 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static error_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__send(message_t *msg);
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__postTask(void );
# 73 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__startOneShot(uint32_t dt);
# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__Send__sendDone(message_t *msg, error_t error);
# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__payloadLength(message_t *msg);
#line 43
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__headerLength(message_t *msg);
#line 65
static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__metadataLength(message_t *msg);
# 85 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/PacketAcknowledgements.nc"
static bool /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketAcknowledgements__wasAcked(
#line 80
message_t * msg);
# 77 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
enum /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0____nesc_unnamed4325 {
#line 77
  PacketLinkLayerP__0__send = 2U
};
#line 77
typedef int /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0____nesc_sillytask_send[/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send];
#line 60
enum /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0____nesc_unnamed4326 {

  PacketLinkLayerP__0__STATE_READY = 0, 
  PacketLinkLayerP__0__STATE_SENDING = 1, 
  PacketLinkLayerP__0__STATE_SENDDONE = 2, 
  PacketLinkLayerP__0__STATE_SIGNAL = 4
};

uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_READY;
message_t */*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg;
uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__totalRetries;






static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__runTask(void );
#line 129
static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__sendDone(message_t *msg, error_t error);










static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__fired(void );
#line 183
static inline link_metadata_t */*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__getMeta(message_t *msg);




static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__setRetries(message_t *msg, uint16_t maxRetries);









static inline uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetries(message_t *msg);




static inline uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetryDelay(message_t *msg);
#line 215
static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__headerLength(message_t *msg);




static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__payloadLength(message_t *msg);
#line 235
static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__metadataLength(message_t *msg);
# 78 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__fired(void );
# 12 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
timer_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__timerid;



uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__last_alarm;


static inline void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__AlarmMilli32Fired(int sig);



static inline error_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Init__init(void );
#line 46
static inline void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__stop(void );
#line 65
static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__startAt(uint32_t t0, uint32_t dt);
#line 126
static uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getNow(void );
#line 141
static inline uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getAlarm(void );
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__postTask(void );
# 109 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
static /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getNow(void );
#line 103
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__startAt(/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type t0, /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type dt);
#line 116
static /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getAlarm(void );
#line 73
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__stop(void );
# 83 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__fired(void );
# 75 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
enum /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0____nesc_unnamed4327 {
#line 75
  AlarmToTimerC__0__fired = 3U
};
#line 75
typedef int /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0____nesc_sillytask_fired[/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired];
#line 54
uint32_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_dt;
bool /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_oneshot;

static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__start(uint32_t t0, uint32_t dt, bool oneshot);
#line 71
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__stop(void );



static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__runTask(void );






static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__fired(void );
#line 98
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__startOneShotAt(uint32_t t0, uint32_t dt);



static inline uint32_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__getNow(void );
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static error_t /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__postTask(void );
# 136 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
static uint32_t /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__getNow(void );
#line 129
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__startOneShotAt(uint32_t t0, uint32_t dt);
#line 78
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__stop(void );




static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__fired(
# 47 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
uint8_t arg_0x2ab95c11ae68);
#line 68
enum /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0____nesc_unnamed4328 {
#line 68
  VirtualizeTimerC__0__updateFromTimer = 4U
};
#line 68
typedef int /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0____nesc_sillytask_updateFromTimer[/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer];
#line 52
enum /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0____nesc_unnamed4329 {
  VirtualizeTimerC__0__NUM_TIMERS = 1U, 
  VirtualizeTimerC__0__END_OF_LIST = 255
};







#line 57
typedef struct /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0____nesc_unnamed4330 {
  uint32_t t0;
  uint32_t dt;
  bool isoneshot : 1;
  bool isrunning : 1;
  bool _reserved : 6;
} /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer_t;

/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer_t /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__m_timers[/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__NUM_TIMERS];




static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__fireTimers(uint32_t now);
#line 93
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__runTask(void );
#line 127
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__fired(void );



static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__startTimer(uint8_t num, uint32_t t0, uint32_t dt, bool isoneshot);
#line 144
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__startOneShot(uint8_t num, uint32_t dt);
#line 180
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__default__fired(uint8_t num);
# 124 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
static inline void SchedulerBasicP__Scheduler__init(void )
{
  /* atomic removed: atomic calls only */
  {
    memset((void *)SchedulerBasicP__m_next, SchedulerBasicP__NO_TASK, sizeof SchedulerBasicP__m_next);
    SchedulerBasicP__m_head = SchedulerBasicP__NO_TASK;
    SchedulerBasicP__m_tail = SchedulerBasicP__NO_TASK;
  }
}

# 57 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Scheduler.nc"
inline static void RealMainP__Scheduler__init(void ){
#line 57
  SchedulerBasicP__Scheduler__init();
#line 57
}
#line 57
# 17 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__set(void ){
#line 17
  HplBcm2835GeneralIOP__Gpio__set(RPI_V2_GPIO_P1_13);
#line 17
}
#line 17
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__set(void )
#line 15
{
#line 15
  /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__set();
}

# 40 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led2__set(void ){
#line 40
  /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__set();
#line 40
}
#line 40
# 109 "/home/wwhuang/git/tinyos-lab11/tos/system/LedsP.nc"
static inline void LedsP__Leds__led2Off(void )
#line 109
{
  LedsP__Led2__set();
  ;
#line 111
  ;
}

# 94 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Leds.nc"
inline static void PlatformP__Leds__led2Off(void ){
#line 94
  LedsP__Leds__led2Off();
#line 94
}
#line 94
# 17 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__set(void ){
#line 17
  HplBcm2835GeneralIOP__Gpio__set(RPI_V2_GPIO_P1_12);
#line 17
}
#line 17
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__set(void )
#line 15
{
#line 15
  /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__set();
}

# 40 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led1__set(void ){
#line 40
  /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__set();
#line 40
}
#line 40
# 94 "/home/wwhuang/git/tinyos-lab11/tos/system/LedsP.nc"
static inline void LedsP__Leds__led1Off(void )
#line 94
{
  LedsP__Led1__set();
  ;
#line 96
  ;
}

# 77 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Leds.nc"
inline static void PlatformP__Leds__led1Off(void ){
#line 77
  LedsP__Leds__led1Off();
#line 77
}
#line 77
# 17 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__set(void ){
#line 17
  HplBcm2835GeneralIOP__Gpio__set(RPI_V2_GPIO_P1_07);
#line 17
}
#line 17
# 15 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__set(void )
#line 15
{
#line 15
  /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__set();
}

# 40 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led0__set(void ){
#line 40
  /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__set();
#line 40
}
#line 40
# 79 "/home/wwhuang/git/tinyos-lab11/tos/system/LedsP.nc"
static inline void LedsP__Leds__led0Off(void )
#line 79
{
  LedsP__Led0__set();
  ;
#line 81
  ;
}

# 61 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Leds.nc"
inline static void PlatformP__Leds__led0Off(void ){
#line 61
  LedsP__Leds__led0Off();
#line 61
}
#line 61
# 18 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc"
static inline void PlatformP__sigint_handler(int sig)
#line 18
{
  do {
#line 19
      flockfile(stdout);
#line 19
      printf("%s:%d:\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc", 19);
#line 19
      printf("Shuting down.\n");
#line 19
      funlockfile(stdout);
    }
  while (
#line 19
  0);
  PlatformP__Leds__led0Off();
  PlatformP__Leds__led1Off();
  PlatformP__Leds__led2Off();

  exit(0);
}

#line 48
static inline error_t PlatformP__InterruptInit__default__init(void )
#line 48
{
  return SUCCESS;
}

# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
inline static error_t PlatformP__InterruptInit__init(void ){
#line 62
  unsigned char __nesc_result;
#line 62

#line 62
  __nesc_result = PlatformP__InterruptInit__default__init();
#line 62

#line 62
  return __nesc_result;
#line 62
}
#line 62
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__makeOutput(void ){
#line 54
  HplBcm2835GeneralIOP__Gpio__makeOutput(RPI_V2_GPIO_P1_13);
#line 54
}
#line 54
# 21 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__makeOutput(void )
#line 21
{
#line 21
  /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__IO__makeOutput();
}

# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led2__makeOutput(void ){
#line 46
  /*PlatformLedsC.Led2Impl*/Bcm2835GpioC__2__GeneralIO__makeOutput();
#line 46
}
#line 46
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__makeOutput(void ){
#line 54
  HplBcm2835GeneralIOP__Gpio__makeOutput(RPI_V2_GPIO_P1_12);
#line 54
}
#line 54
# 21 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__makeOutput(void )
#line 21
{
#line 21
  /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__IO__makeOutput();
}

# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led1__makeOutput(void ){
#line 46
  /*PlatformLedsC.Led1Impl*/Bcm2835GpioC__1__GeneralIO__makeOutput();
#line 46
}
#line 46
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__makeOutput(void ){
#line 54
  HplBcm2835GeneralIOP__Gpio__makeOutput(RPI_V2_GPIO_P1_07);
#line 54
}
#line 54
# 21 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__makeOutput(void )
#line 21
{
#line 21
  /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__IO__makeOutput();
}

# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void LedsP__Led0__makeOutput(void ){
#line 46
  /*PlatformLedsC.Led0Impl*/Bcm2835GpioC__0__GeneralIO__makeOutput();
#line 46
}
#line 46
# 56 "/home/wwhuang/git/tinyos-lab11/tos/system/LedsP.nc"
static inline error_t LedsP__Init__init(void )
#line 56
{
  /* atomic removed: atomic calls only */
#line 57
  {
    ;
    LedsP__Led0__makeOutput();
    LedsP__Led1__makeOutput();
    LedsP__Led2__makeOutput();
    LedsP__Led0__set();
    LedsP__Led1__set();
    LedsP__Led2__set();
  }
  return SUCCESS;
}

# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
inline static error_t PlatformP__LedsInit__init(void ){
#line 62
  unsigned char __nesc_result;
#line 62

#line 62
  __nesc_result = LedsP__Init__init();
#line 62

#line 62
  return __nesc_result;
#line 62
}
#line 62
# 42 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
static inline error_t HplBcm2835GeneralIOP__Init__init(void )
#line 42
{
  int mem_fd;
  void *gpio_map;


  mem_fd = open("/dev/mem", 02 | 04010000);
  if (mem_fd == -1) {
      do {
#line 49
          flockfile(stderr);
#line 49
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc", 49);
#line 49
          fprintf(stderr, "Could not open /dev/mem\n");
#line 49
          funlockfile(stderr);
        }
      while (
#line 49
      0);
      do {
#line 50
          flockfile(stderr);
#line 50
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc", 50);
#line 50
          fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 50
          funlockfile(stderr);
        }
      while (
#line 50
      0);
      exit(1);
    }


  gpio_map = mmap(
  (void *)0, 
  4 * 1024, 
  0x1 | 0x2, 
  0x01, 
  mem_fd, 
  0x20000000 + 0x200000);

  if (gpio_map == (void *)-1) {
      do {
#line 64
          flockfile(stderr);
#line 64
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc", 64);
#line 64
          fprintf(stderr, "mmap of gpio failed.\n");
#line 64
          funlockfile(stderr);
        }
      while (
#line 64
      0);
      do {
#line 65
          flockfile(stderr);
#line 65
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc", 65);
#line 65
          fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 65
          funlockfile(stderr);
        }
      while (
#line 65
      0);
      exit(1);
    }

  close(mem_fd);


  HplBcm2835GeneralIOP__gpio = (volatile unsigned *)gpio_map;

  return SUCCESS;
}

# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
inline static error_t PlatformP__GpioInit__init(void ){
#line 62
  unsigned char __nesc_result;
#line 62

#line 62
  __nesc_result = HplBcm2835GeneralIOP__Init__init();
#line 62

#line 62
  return __nesc_result;
#line 62
}
#line 62
# 27 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc"
static inline error_t PlatformP__Init__init(void )
#line 27
{

  setvbuf(stdout, (void *)0, 2, 0);

  do {
#line 31
      flockfile(stdout);
#line 31
      printf("%s:%d:\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc", 31);
#line 31
      printf("Bringing system online.\n");
#line 31
      funlockfile(stdout);
    }
  while (
#line 31
  0);
  PlatformP__GpioInit__init();
  PlatformP__LedsInit__init();
  PlatformP__InterruptInit__init();

  do {
#line 36
      flockfile(stdout);
#line 36
      printf("%s:%d:\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/platforms/rpi/PlatformP.nc", 36);
#line 36
      printf("Setting SIGINT signal handler.\n");
#line 36
      funlockfile(stdout);
    }
  while (
#line 36
  0);
  signal(2, PlatformP__sigint_handler);

  return SUCCESS;
}

# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
inline static error_t RealMainP__PlatformInit__init(void ){
#line 62
  unsigned char __nesc_result;
#line 62

#line 62
  __nesc_result = PlatformP__Init__init();
#line 62

#line 62
  return __nesc_result;
#line 62
}
#line 62
# 65 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Scheduler.nc"
inline static bool RealMainP__Scheduler__runNextTask(void ){
#line 65
  unsigned char __nesc_result;
#line 65

#line 65
  __nesc_result = SchedulerBasicP__Scheduler__runNextTask();
#line 65

#line 65
  return __nesc_result;
#line 65
}
#line 65
# 103 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
inline static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__startAt(/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type t0, /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type dt){
#line 103
  /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__startAt(t0, dt);
#line 103
}
#line 103
# 57 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__start(uint32_t t0, uint32_t dt, bool oneshot)
#line 57
{
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_dt = dt;
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_oneshot = oneshot;
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__startAt(t0, dt);
}

#line 98
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__startOneShotAt(uint32_t t0, uint32_t dt)
#line 98
{
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__start(t0, dt, TRUE);
}

# 129 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__startOneShotAt(uint32_t t0, uint32_t dt){
#line 129
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__startOneShotAt(t0, dt);
#line 129
}
#line 129
# 46 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
static inline void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__stop(void )
#line 46
{
  struct itimerspec new_timer = { { 0, 0 }, { 0, 0 } };

#line 48
  timer_settime(/*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__timerid, 1, &new_timer, (void *)0);
}

# 73 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
inline static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__stop(void ){
#line 73
  /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__stop();
#line 73
}
#line 73
# 71 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__stop(void )
#line 71
{
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__stop();
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__stop(void ){
#line 78
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__stop();
#line 78
}
#line 78
# 109 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
inline static /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getNow(void ){
#line 109
  unsigned int __nesc_result;
#line 109

#line 109
  __nesc_result = /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getNow();
#line 109

#line 109
  return __nesc_result;
#line 109
}
#line 109
# 102 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
static inline uint32_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__getNow(void )
#line 102
{
  return /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getNow();
}

# 136 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static uint32_t /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__getNow(void ){
#line 136
  unsigned int __nesc_result;
#line 136

#line 136
  __nesc_result = /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__getNow();
#line 136

#line 136
  return __nesc_result;
#line 136
}
#line 136
# 93 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__runTask(void )
#line 93
{




  uint32_t now = /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__getNow();
  int32_t min_remaining = (1UL << 31) - 1;
  bool min_remaining_isset = FALSE;
  uint16_t num;

  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__stop();

  for (num = 0; num < /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__NUM_TIMERS; num++) {
      /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer_t *timer = &/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__m_timers[num];

      if (timer->isrunning) {
          uint32_t elapsed = now - timer->t0;
          int32_t remaining = timer->dt - elapsed;

          if (remaining < min_remaining) {
              min_remaining = remaining;
              min_remaining_isset = TRUE;
            }
        }
    }

  if (min_remaining_isset) {
      if (min_remaining <= 0) {
        /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__fireTimers(now);
        }
      else {
#line 123
        /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__startOneShotAt(now, min_remaining);
        }
    }
}

# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
inline static error_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__postTask(void ){
#line 67
  unsigned char __nesc_result;
#line 67

#line 67
  __nesc_result = SchedulerBasicP__TaskBasic__postTask(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send);
#line 67

#line 67
  return __nesc_result;
#line 67
}
#line 67
# 140 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__fired(void )
{
  for (; 0; ) ;

  /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__postTask();
}

# 180 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__default__fired(uint8_t num)
#line 180
{
}

# 83 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__fired(uint8_t arg_0x2ab95c11ae68){
#line 83
  switch (arg_0x2ab95c11ae68) {
#line 83
    case 0U:
#line 83
      /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__fired();
#line 83
      break;
#line 83
    default:
#line 83
      /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__default__fired(arg_0x2ab95c11ae68);
#line 83
      break;
#line 83
    }
#line 83
}
#line 83
# 97 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
static inline bool SchedulerBasicP__isWaiting(uint8_t id)
{
  return SchedulerBasicP__m_next[id] != SchedulerBasicP__NO_TASK || SchedulerBasicP__m_tail == id;
}

static inline bool SchedulerBasicP__pushTask(uint8_t id)
{
  if (!SchedulerBasicP__isWaiting(id)) 
    {
      if (SchedulerBasicP__m_head == SchedulerBasicP__NO_TASK) 
        {
          SchedulerBasicP__m_head = id;
          SchedulerBasicP__m_tail = id;
        }
      else 
        {
          SchedulerBasicP__m_next[SchedulerBasicP__m_tail] = id;
          SchedulerBasicP__m_tail = id;
        }
      return TRUE;
    }
  else 
    {
      return FALSE;
    }
}

# 127 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__fired(void )
#line 127
{
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__fireTimers(/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__getNow());
}

# 83 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__fired(void ){
#line 83
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__fired();
#line 83
}
#line 83
# 141 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
static inline uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getAlarm(void )
#line 141
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 142
    {
      unsigned int __nesc_temp = 
#line 142
      /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__last_alarm;

      {
#line 142
        __nesc_atomic_end(__nesc_atomic); 
#line 142
        return __nesc_temp;
      }
    }
#line 144
    __nesc_atomic_end(__nesc_atomic); }
}

# 116 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
inline static /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__size_type /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getAlarm(void ){
#line 116
  unsigned int __nesc_result;
#line 116

#line 116
  __nesc_result = /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getAlarm();
#line 116

#line 116
  return __nesc_result;
#line 116
}
#line 116
# 75 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__runTask(void )
#line 75
{
  if (/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_oneshot == FALSE) {
    /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__start(/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__getAlarm(), /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__m_dt, FALSE);
    }
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Timer__fired();
}

# 292 "/usr/local/lib/ncc/nesc_nx.h"
static __inline  uint8_t __nesc_ntoh_leuint8(const void * source)
#line 292
{
  const uint8_t *base = source;

#line 294
  return base[0];
}

# 11 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAm154DummyP.nc"
static inline void CC2520RpiAm154DummyP__Ieee154Send__sendDone(message_t *msg, error_t error)
#line 11
{
}

# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Send__sendDone(message_t *msg, error_t error){
#line 54
  CC2520RpiAm154DummyP__Ieee154Send__sendDone(msg, error);
#line 54
}
#line 54
# 281 "/usr/local/lib/ncc/nesc_nx.h"
static __inline  uint8_t __nesc_ntoh_uint8(const void * source)
#line 281
{
  const uint8_t *base = source;

#line 283
  return base[0];
}

# 43 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__headerLength(message_t *msg){
#line 43
  unsigned char __nesc_result;
#line 43

#line 43
  __nesc_result = /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__headerLength(msg);
#line 43

#line 43
  return __nesc_result;
#line 43
}
#line 43
# 67 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline activemessage_header_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getHeader(message_t *msg)
{
  return (void *)msg + /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__headerLength(msg);
}

#line 199
static __inline am_id_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__type(message_t *msg)
{
  return __nesc_ntoh_uint8(/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getHeader(msg)->type.nxdata);
}

#line 108
static inline void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__default__sendDone(am_id_t id, message_t *msg, error_t error)
{
}

# 110 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/AMSend.nc"
inline static void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__sendDone(am_id_t arg_0x2ab95bea73c8, message_t * msg, error_t error){
#line 110
    /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__default__sendDone(arg_0x2ab95bea73c8, msg, error);
#line 110
}
#line 110
# 98 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static __inline void /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubSend__sendDone(message_t *msg, error_t error)
{
  /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMSend__sendDone(/*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__type(msg), msg, error);
}

# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosSend__sendDone(message_t *msg, error_t error){
#line 54
  /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubSend__sendDone(msg, error);
#line 54
}
#line 54
# 16 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmPacketP.nc"
static inline uint8_t CC2520RpiAmPacketP__RadioPacket__headerLength(message_t *msg)
#line 16
{



  return sizeof(cc2520_header_t );
}

# 43 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__headerLength(message_t *msg){
#line 43
  unsigned char __nesc_result;
#line 43

#line 43
  __nesc_result = CC2520RpiAmPacketP__RadioPacket__headerLength(msg);
#line 43

#line 43
  return __nesc_result;
#line 43
}
#line 43
# 215 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__headerLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__headerLength(msg);
}

# 43 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__headerLength(message_t *msg){
#line 43
  unsigned char __nesc_result;
#line 43

#line 43
  __nesc_result = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__headerLength(msg);
#line 43

#line 43
  return __nesc_result;
#line 43
}
#line 43
# 282 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__headerLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__headerLength(msg) + sizeof(ieee154_simple_header_t );
}

# 43 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__headerLength(message_t *msg){
#line 43
  unsigned char __nesc_result;
#line 43

#line 43
  __nesc_result = /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__headerLength(msg);
#line 43

#line 43
  return __nesc_result;
#line 43
}
#line 43
# 127 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/TinyosNetworkLayerC.nc"
static inline network_header_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__getHeader(message_t *msg)
{
  return (void *)msg + /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__headerLength(msg);
}

#line 214
static inline void /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubSend__sendDone(message_t *msg, error_t result)
{
  if (__nesc_ntoh_leuint8(/*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__getHeader(msg)->network.nxdata) == 0x3f) {
    /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosSend__sendDone(msg, result);
    }
  else {
#line 219
    /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Send__sendDone(msg, result);
    }
}

# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static void CC2520RpiAmUniqueP__Send__sendDone(message_t *msg, error_t error){
#line 54
  /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubSend__sendDone(msg, error);
#line 54
}
#line 54
# 33 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmUniqueP.nc"
static inline void CC2520RpiAmUniqueP__SubSend__sendDone(message_t *msg, error_t error)
#line 33
{
  CC2520RpiAmUniqueP__Send__sendDone(msg, error);
}

# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__Send__sendDone(message_t *msg, error_t error){
#line 54
  CC2520RpiAmUniqueP__SubSend__sendDone(msg, error);
#line 54
}
#line 54
# 50 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmPacketP.nc"
static inline uint8_t CC2520RpiAmPacketP__RadioPacket__metadataLength(message_t *msg)
#line 50
{
  return 0;
}

# 65 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__metadataLength(message_t *msg){
#line 65
  unsigned char __nesc_result;
#line 65

#line 65
  __nesc_result = CC2520RpiAmPacketP__RadioPacket__metadataLength(msg);
#line 65

#line 65
  return __nesc_result;
#line 65
}
#line 65
# 235 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__metadataLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__metadataLength(msg) + sizeof(link_metadata_t );
}

#line 183
static inline link_metadata_t */*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__getMeta(message_t *msg)
{
  return (void *)msg + sizeof(message_t ) - /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__metadataLength(msg);
}

static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__setRetries(message_t *msg, uint16_t maxRetries)
{
  /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__getMeta(msg)->maxRetries = maxRetries;
}

# 235 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc"
static inline error_t CC2520RpiSendP__BareSend__send(message_t *msg)
#line 235
{
  CC2520RpiSendP__write_fifo_header_t whdr;
  ssize_t ret;

#line 266
  whdr.ptr_to_msg = msg;
  whdr.len = ((uint8_t *)msg)[0];
  ret = write(CC2520RpiSendP__cc2520_write, &whdr, sizeof(CC2520RpiSendP__write_fifo_header_t ));
  if (ret == -1) {
      do {
#line 270
          flockfile(stderr);
#line 270
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 270);
#line 270
          fprintf(stderr, "could not write to fifo.\n");
#line 270
          funlockfile(stderr);
        }
      while (
#line 270
      0);
    }



  ret = write(CC2520RpiSendP__cc2520_write, (uint8_t *)msg + 1, whdr.len - 2);
  if (ret == -1) {
      do {
#line 277
          flockfile(stderr);
#line 277
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 277);
#line 277
          fprintf(stderr, "could not write to fifo.\n");
#line 277
          funlockfile(stderr);
        }
      while (
#line 277
      0);
    }

  ;

  return SUCCESS;
}

# 46 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static error_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__send(message_t *msg){
#line 46
  unsigned char __nesc_result;
#line 46

#line 46
  __nesc_result = CC2520RpiSendP__BareSend__send(msg);
#line 46

#line 46
  return __nesc_result;
#line 46
}
#line 46
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
inline static error_t /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__postTask(void ){
#line 67
  unsigned char __nesc_result;
#line 67

#line 67
  __nesc_result = SchedulerBasicP__TaskBasic__postTask(/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer);
#line 67

#line 67
  return __nesc_result;
#line 67
}
#line 67
# 131 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__startTimer(uint8_t num, uint32_t t0, uint32_t dt, bool isoneshot)
#line 131
{
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer_t *timer = &/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__m_timers[num];

#line 133
  timer->t0 = t0;
  timer->dt = dt;
  timer->isoneshot = isoneshot;
  timer->isrunning = TRUE;
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__postTask();
}





static inline void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__startOneShot(uint8_t num, uint32_t dt)
#line 144
{
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__startTimer(num, /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__TimerFrom__getNow(), dt, TRUE);
}

# 73 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Timer.nc"
inline static void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__startOneShot(uint32_t dt){
#line 73
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__startOneShot(0U, dt);
#line 73
}
#line 73
# 203 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetryDelay(message_t *msg)
{
  return /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__getMeta(msg)->retryDelay;
}

# 110 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmRadioP.nc"
static inline bool CC2520RpiAmRadioP__PacketAcknowledgements__wasAcked(message_t *msg)
#line 110
{
  return TRUE;
}

# 85 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/PacketAcknowledgements.nc"
inline static bool /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketAcknowledgements__wasAcked(message_t * msg){
#line 85
  unsigned char __nesc_result;
#line 85

#line 85
  __nesc_result = CC2520RpiAmRadioP__PacketAcknowledgements__wasAcked(msg);
#line 85

#line 85
  return __nesc_result;
#line 85
}
#line 85
# 198 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline uint16_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetries(message_t *msg)
{
  return /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__getMeta(msg)->maxRetries;
}

#line 77
static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__runTask(void )
{
  uint16_t retries;

  for (; 0; ) ;

  retries = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetries(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg);

  if (/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state == /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SENDDONE) 
    {
      if (retries == 0 || /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketAcknowledgements__wasAcked(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg)) {
        /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SIGNAL + SUCCESS;
        }
      else {
#line 89
        if (++/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__totalRetries < retries) 
          {
            uint16_t delay;

            /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SENDING;
            delay = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__getRetryDelay(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg);

            if (delay > 0) 
              {
                /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__DelayTimer__startOneShot(delay);
                return;
              }
          }
        else {
          /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SIGNAL + FAIL;
          }
        }
    }
#line 106
  if (/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state == /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SENDING) 
    {
      /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SENDDONE;

      if (/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__send(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg) != SUCCESS) {
        /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__postTask();
        }
      return;
    }

  if (/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state >= /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SIGNAL) 
    {
      error_t error = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state - /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SIGNAL;


      if (retries > 0) {
        /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__PacketLink__setRetries(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg, /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__totalRetries);
        }
      /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_READY;
      /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__Send__sendDone(/*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__currentMsg, error);
    }
}

static inline void /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__sendDone(message_t *msg, error_t error)
{
  for (; 0; ) ;
  for (; 0; ) ;

  if (error != SUCCESS) {
    /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__state = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__STATE_SIGNAL + error;
    }
  /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__postTask();
}

# 54 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareSend.nc"
inline static void CC2520RpiSendP__BareSend__sendDone(message_t *msg, error_t error){
#line 54
  /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubSend__sendDone(msg, error);
#line 54
}
#line 54
# 249 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
static inline ack_metadata_t *CC2520RpiRadioP__getMetaAck(message_t *msg)
#line 249
{
  return & ((cc2520packet_metadata_t *)msg->metadata)->ack;
}

#line 312
static inline void CC2520RpiRadioP__PacketMetadata__setWasAcked(message_t *msg, bool ack)
#line 312
{
  CC2520RpiRadioP__getMetaAck(msg)->ack = ack;
}

# 85 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
inline static void CC2520RpiSendP__PacketMetadata__setWasAcked(message_t *msg, bool ack){
#line 85
  CC2520RpiRadioP__PacketMetadata__setWasAcked(msg, ack);
#line 85
}
#line 85
# 53 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc"
static inline void CC2520RpiSendP__sendDone_task__runTask(void )
#line 53
{

  switch (CC2520RpiSendP__send_hdr.ret) {
      case 255: 
        case 253: 
          case 252: 
            ;
      CC2520RpiSendP__PacketMetadata__setWasAcked(CC2520RpiSendP__send_hdr.ptr_to_msg, FALSE);
      break;
      case 254: 
        do {
#line 63
            flockfile(stderr);
#line 63
            fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 63);
#line 63
            fprintf(stderr, "INCORRECT LENGTH\n");
#line 63
            funlockfile(stderr);
          }
        while (
#line 63
        0);
      break;
      case 0: 
        CC2520RpiSendP__PacketMetadata__setWasAcked(CC2520RpiSendP__send_hdr.ptr_to_msg, TRUE);
      break;
      default: 
        if (CC2520RpiSendP__send_hdr.ret == CC2520RpiSendP__send_hdr.len - 1) {
            CC2520RpiSendP__PacketMetadata__setWasAcked(CC2520RpiSendP__send_hdr.ptr_to_msg, TRUE);
          }
        else 
#line 71
          {
            do {
#line 72
                flockfile(stderr);
#line 72
                fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 72);
#line 72
                fprintf(stderr, "write() weird return code: %i\n", CC2520RpiSendP__send_hdr.ret);
#line 72
                funlockfile(stderr);
              }
            while (
#line 72
            0);
          }
      break;
    }

  CC2520RpiSendP__BareSend__sendDone(CC2520RpiSendP__send_hdr.ptr_to_msg, SUCCESS);
}

# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
inline static message_t *CC2520RpiReceiveP__BareReceive__receive(message_t *msg){
#line 42
  nx_struct message_t *__nesc_result;
#line 42

#line 42
  __nesc_result = /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubReceive__receive(msg);
#line 42

#line 42
  return __nesc_result;
#line 42
}
#line 42
# 27 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/time/UnixTimeP.nc"
static inline uint64_t UnixTimeP__UnixTime__getMicroseconds(void )
#line 27
{
  int ret;
  struct timeval tv;
  uint64_t unix_time = 0;

  ret = gettimeofday(&tv, (void *)0);
  if (ret == 0) {
      unix_time = (uint64_t )tv.tv_sec * 1000000 + (uint64_t )tv.tv_usec;
    }
  return unix_time;
}

# 5 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/time/UnixTime.nc"
inline static uint64_t CC2520RpiReceiveP__UnixTime__getMicroseconds(void ){
#line 5
  unsigned long long __nesc_result;
#line 5

#line 5
  __nesc_result = UnixTimeP__UnixTime__getMicroseconds();
#line 5

#line 5
  return __nesc_result;
#line 5
}
#line 5
# 253 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
static inline timestamp_metadata_t *CC2520RpiRadioP__getMetaTimestamp(message_t *msg)
#line 253
{
  return & ((cc2520packet_metadata_t *)msg->metadata)->timestamp;
}

#line 277
static inline void CC2520RpiRadioP__PacketMetadata__setTimestamp(message_t *msg, uint64_t t)
#line 277
{
  CC2520RpiRadioP__getMetaTimestamp(msg)->timestamp_micro = t;
}

# 13 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
inline static void CC2520RpiReceiveP__PacketMetadata__setTimestamp(message_t *msg, uint64_t timestamp){
#line 13
  CC2520RpiRadioP__PacketMetadata__setTimestamp(msg, timestamp);
#line 13
}
#line 13
# 245 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
static inline cc2520_metadata_t *CC2520RpiRadioP__getMetaCC2520(message_t *msg)
#line 245
{
  return & ((cc2520packet_metadata_t *)msg->metadata)->cc2520;
}

#line 273
static inline void CC2520RpiRadioP__PacketMetadata__setRssi(message_t *msg, uint8_t rssi)
#line 273
{
  CC2520RpiRadioP__getMetaCC2520(msg)->rssi = rssi;
}

# 8 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
inline static void CC2520RpiReceiveP__PacketMetadata__setRssi(message_t *msg, uint8_t rssi){
#line 8
  CC2520RpiRadioP__PacketMetadata__setRssi(msg, rssi);
#line 8
}
#line 8
# 269 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
static inline void CC2520RpiRadioP__PacketMetadata__setLqi(message_t *msg, uint8_t lqi)
#line 269
{
  CC2520RpiRadioP__getMetaCC2520(msg)->lqi = lqi;
}

# 7 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/PacketMetadata.nc"
inline static void CC2520RpiReceiveP__PacketMetadata__setLqi(message_t *msg, uint8_t lqi){
#line 7
  CC2520RpiRadioP__PacketMetadata__setLqi(msg, lqi);
#line 7
}
#line 7
# 51 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc"
static inline void CC2520RpiReceiveP__receive_task__runTask(void )
#line 51
{
  uint8_t rssi;
#line 52
  uint8_t crc_lqi;


  rssi = CC2520RpiReceiveP__rx_msg_ptr[CC2520RpiReceiveP__rx_msg_ptr[0] - 1];
  crc_lqi = CC2520RpiReceiveP__rx_msg_ptr[CC2520RpiReceiveP__rx_msg_ptr[0]];
  if ((crc_lqi >> 7) == 0) {
      ;

      return;
    }
  CC2520RpiReceiveP__PacketMetadata__setLqi((message_t *)CC2520RpiReceiveP__rx_msg_ptr, crc_lqi & 0x7F);
  CC2520RpiReceiveP__PacketMetadata__setRssi((message_t *)CC2520RpiReceiveP__rx_msg_ptr, rssi);


  CC2520RpiReceiveP__PacketMetadata__setTimestamp((message_t *)CC2520RpiReceiveP__rx_msg_ptr, 
  CC2520RpiReceiveP__UnixTime__getMicroseconds());
#line 97
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 97
    CC2520RpiReceiveP__rx_msg_ptr = (uint8_t *)
    CC2520RpiReceiveP__BareReceive__receive((message_t *)CC2520RpiReceiveP__rx_msg_ptr);
#line 98
    __nesc_atomic_end(__nesc_atomic); }
}

# 156 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return msg;
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
inline static message_t * /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__receive(am_id_t arg_0x2ab95bea2488, message_t * msg, void * payload, uint8_t len){
#line 78
  nx_struct message_t *__nesc_result;
#line 78

#line 78
    __nesc_result = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__default__receive(arg_0x2ab95bea2488, msg, payload, len);
#line 78

#line 78
  return __nesc_result;
#line 78
}
#line 78
# 151 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SnoopDefault__receive(id, msg, payload, len);
#line 153
  ;
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
inline static message_t * /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__receive(am_id_t arg_0x2ab95bea51a0, message_t * msg, void * payload, uint8_t len){
#line 78
  nx_struct message_t *__nesc_result;
#line 78

#line 78
    __nesc_result = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__default__receive(arg_0x2ab95bea51a0, msg, payload, len);
#line 78

#line 78
  return __nesc_result;
#line 78
}
#line 78
# 146 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return msg;
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
inline static message_t * /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__receive(am_id_t arg_0x2ab95bea48b0, message_t * msg, void * payload, uint8_t len){
#line 78
  nx_struct message_t *__nesc_result;
#line 78

#line 78
    __nesc_result = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__default__receive(arg_0x2ab95bea48b0, msg, payload, len);
#line 78

#line 78
  return __nesc_result;
#line 78
}
#line 78
# 303 "/usr/local/lib/ncc/nesc_nx.h"
static __inline  int8_t __nesc_ntoh_int8(const void * source)
#line 303
{
#line 303
  return __nesc_ntoh_uint8(source);
}

#line 340
static __inline  uint32_t __nesc_ntoh_uint32(const void * source)
#line 340
{
  const uint8_t *base = source;

#line 342
  return ((((uint32_t )base[0] << 24) | (
  (uint32_t )base[1] << 16)) | (
  (uint32_t )base[2] << 8)) | base[3];
}

# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/lib/printf/printf.h"
static inline void printfflush()
#line 9
{
}

# 26 "OpoBaseP.nc"
static inline message_t *OpoBaseP__OpoReceive__receive(message_t *msg, void *payload, uint8_t len)
#line 26
{
  opo_bmsg_t *data = (opo_bmsg_t *)payload;
  int i;
  uint32_t total_time = 0;

#line 30
  printf("RANGE:%u\n", __nesc_ntoh_uint32(data->rec.range.nxdata));
  printf("SEQ:%u\n", __nesc_ntoh_uint32(data->rec.sequence.nxdata));
  printf("RX_ID:0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->m_id[i].nxdata));
    }
  printf("\n");
  printf("TX_ID:0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->rec.o_id[i].nxdata));
    }
  printf("\n");
  printf("RSSI:%i\n", __nesc_ntoh_int8(data->rec.rssi.nxdata));
  printf("TX_PWR:%u\n", __nesc_ntoh_uint8(data->rec.tx_pwr.nxdata));
  total_time += __nesc_ntoh_uint8(data->rec.seconds.nxdata);
  total_time += __nesc_ntoh_uint8(data->rec.minutes.nxdata) * 60;
  total_time += __nesc_ntoh_uint8(data->rec.hours.nxdata) * 60 * 60;
  printf("TIME:%u\n", total_time);
  printfflush();
  return msg;
}

static inline message_t *OpoBaseP__BlinkReceive__receive(message_t *msg, void *payload, uint8_t len)
#line 52
{
  oblink_base_msg_t *data = (oblink_base_msg_t *)payload;
  int i;

  printf("Range: %u\n", __nesc_ntoh_uint32(data->range.nxdata));
  printf("M_ID: 0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->m_id[i].nxdata));
    }
  printf("\n");
  printf("O_ID: 0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->o_id[i].nxdata));
    }
  printf("\n");

  printfflush();
  return msg;
}

# 310 "/usr/local/lib/ncc/nesc_nx.h"
static __inline  uint16_t __nesc_ntoh_uint16(const void * source)
#line 310
{
  const uint8_t *base = source;

#line 312
  return ((uint16_t )base[0] << 8) | base[1];
}

# 72 "OpoBaseP.nc"
static inline message_t *OpoBaseP__VisualReceive__receive(message_t *msg, void *payload, uint8_t len)
#line 72
{
  ovis_base_msg_t *data = (ovis_base_msg_t *)payload;
  int i;

  printf("22 ");

  printf("0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->rx_id[i].nxdata));
    }
  printf(" ");

  printf("0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->tx_id[i].nxdata));
    }
  printf(" ");

  printf("%u ", __nesc_ntoh_uint16(data->full_time.nxdata));

  printf("%u ", __nesc_ntoh_uint16(data->ultrasonic_rf_dt.nxdata));
  printf("%u ", __nesc_ntoh_uint16(data->ultrasonic_wake_dt.nxdata));
  printf("%u ", __nesc_ntoh_uint16(data->ultrasonic_dt.nxdata));
  printf("%u ", __nesc_ntoh_uint16(data->seq.nxdata));
  printf("%u ", __nesc_ntoh_uint16(data->rx_fails.nxdata));
  printf("%u\n", __nesc_ntoh_uint16(data->tx_fails.nxdata));

  printfflush();
  return msg;
}

static inline message_t *OpoBaseP__RxReceive__receive(message_t *msg, void *payload, uint8_t len)
#line 103
{
  opo_rx_base_msg_t *data = (opo_rx_base_msg_t *)payload;
  int i;

  printf("0x");
  for (i = 0; i < 6; i++) {
      printf("%x", __nesc_ntoh_uint8(data->rx_id[i].nxdata));
    }
  printf(" %u", __nesc_ntoh_uint16(data->t_rf.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic_wake.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic_wake_falling.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic_falling.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic_wake_falling.nxdata) - __nesc_ntoh_uint16(data->t_ultrasonic_wake.nxdata));
  printf(" %u", __nesc_ntoh_uint16(data->t_ultrasonic_falling.nxdata) - __nesc_ntoh_uint16(data->t_ultrasonic.nxdata));
  printf("\n");

  printfflush();
  return msg;
}

static inline message_t *OpoBaseP__ADCReceive__receive(message_t *msg, void *payload, uint8_t len)
#line 124
{
  opo_adc_msg_t *data = (opo_adc_msg_t *)payload;
  int i;

  for (i = 0; i < 6; i++) {
      printf("24 %u %u %u \n", __nesc_ntoh_uint16(data->rf_time.nxdata), __nesc_ntoh_uint16(data->readings[i].nxdata), __nesc_ntoh_uint16(data->times[i].nxdata));
    }

  printf("----------------------\n");

  return msg;
}

# 141 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__default__receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ReceiveDefault__receive(id, msg, payload, len);
#line 143
  ;
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Receive.nc"
inline static message_t * /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__receive(am_id_t arg_0x2ab95bea65f8, message_t * msg, void * payload, uint8_t len){
#line 78
  nx_struct message_t *__nesc_result;
#line 78

#line 78
  switch (arg_0x2ab95bea65f8) {
#line 78
    case 20:
#line 78
      __nesc_result = OpoBaseP__OpoReceive__receive(msg, payload, len);
#line 78
      break;
#line 78
    case 21:
#line 78
      __nesc_result = OpoBaseP__BlinkReceive__receive(msg, payload, len);
#line 78
      break;
#line 78
    case 22:
#line 78
      __nesc_result = OpoBaseP__VisualReceive__receive(msg, payload, len);
#line 78
      break;
#line 78
    case 23:
#line 78
      __nesc_result = OpoBaseP__RxReceive__receive(msg, payload, len);
#line 78
      break;
#line 78
    case 24:
#line 78
      __nesc_result = OpoBaseP__ADCReceive__receive(msg, payload, len);
#line 78
      break;
#line 78
    default:
#line 78
      __nesc_result = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__default__receive(arg_0x2ab95bea65f8, msg, payload, len);
#line 78
      break;
#line 78
    }
#line 78

#line 78
  return __nesc_result;
#line 78
}
#line 78
# 120 "/home/wwhuang/git/tinyos-lab11/tos/system/ActiveMessageAddressC.nc"
static inline am_addr_t ActiveMessageAddressC__amAddress(void )
#line 120
{
  am_addr_t myAddr;

  /* atomic removed: atomic calls only */
#line 122
  myAddr = ActiveMessageAddressC__addr;
  return myAddr;
}

#line 73
static inline am_addr_t ActiveMessageAddressC__ActiveMessageAddress__amAddress(void )
#line 73
{
  return ActiveMessageAddressC__amAddress();
}

# 50 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/ActiveMessageAddress.nc"
inline static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ActiveMessageAddress__amAddress(void ){
#line 50
  unsigned short __nesc_result;
#line 50

#line 50
  __nesc_result = ActiveMessageAddressC__ActiveMessageAddress__amAddress();
#line 50

#line 50
  return __nesc_result;
#line 50
}
#line 50
# 163 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static __inline am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__address(void )
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__ActiveMessageAddress__amAddress();
}

# 322 "/usr/local/lib/ncc/nesc_nx.h"
static __inline  uint16_t __nesc_ntoh_leuint16(const void * source)
#line 322
{
  const uint8_t *base = source;

#line 324
  return ((uint16_t )base[1] << 8) | base[0];
}

# 77 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayerP.nc"
static inline ieee154_simple_header_t */*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__getHeader(message_t *msg)
{
  return (void *)msg + /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__headerLength(msg);
}

#line 178
static inline uint16_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__Ieee154PacketLayer__getDestAddr(message_t *msg)
{
  return __nesc_ntoh_leuint16(/*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__getHeader(msg)->dest.nxdata);
}

# 141 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayer.nc"
inline static uint16_t CC2520RpiAmRadioP__Ieee154PacketLayer__getDestAddr(message_t *msg){
#line 141
  unsigned short __nesc_result;
#line 141

#line 141
  __nesc_result = /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__Ieee154PacketLayer__getDestAddr(msg);
#line 141

#line 141
  return __nesc_result;
#line 141
}
#line 141
# 219 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmRadioP.nc"
static inline am_addr_t CC2520RpiAmRadioP__ActiveMessageConfig__destination(message_t *msg)
{
  return CC2520RpiAmRadioP__Ieee154PacketLayer__getDestAddr(msg);
}

# 40 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageConfig.nc"
inline static am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Config__destination(message_t *msg){
#line 40
  unsigned short __nesc_result;
#line 40

#line 40
  __nesc_result = CC2520RpiAmRadioP__ActiveMessageConfig__destination(msg);
#line 40

#line 40
  return __nesc_result;
#line 40
}
#line 40
# 179 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static __inline am_addr_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__destination(message_t *msg)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Config__destination(msg);
}

#line 173
static __inline bool /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__isForMe(message_t *msg)
{
  am_addr_t addr = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__destination(msg);

#line 176
  return addr == /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__address() || addr == AM_BROADCAST_ADDR;
}

# 23 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmPacketP.nc"
static inline uint8_t CC2520RpiAmPacketP__RadioPacket__payloadLength(message_t *msg)
#line 23
{




  uint8_t len = __nesc_ntoh_leuint8(((cc2520packet_header_t *)msg->header)->cc2520.length.nxdata);

  return len - 2;
}

# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__payloadLength(message_t *msg){
#line 49
  unsigned char __nesc_result;
#line 49

#line 49
  __nesc_result = CC2520RpiAmPacketP__RadioPacket__payloadLength(msg);
#line 49

#line 49
  return __nesc_result;
#line 49
}
#line 49
# 220 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/PacketLinkLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__payloadLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__SubPacket__payloadLength(msg);
}

# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__payloadLength(message_t *msg){
#line 49
  unsigned char __nesc_result;
#line 49

#line 49
  __nesc_result = /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__RadioPacket__payloadLength(msg);
#line 49

#line 49
  return __nesc_result;
#line 49
}
#line 49
# 287 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/Ieee154PacketLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__payloadLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__SubPacket__payloadLength(msg) - sizeof(ieee154_simple_header_t );
}

# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__payloadLength(message_t *msg){
#line 49
  unsigned char __nesc_result;
#line 49

#line 49
  __nesc_result = /*CC2520RpiAmRadioC.Ieee154PacketLayerC.Ieee154PacketLayerP*/Ieee154PacketLayerP__0__RadioPacket__payloadLength(msg);
#line 49

#line 49
  return __nesc_result;
#line 49
}
#line 49
# 159 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/TinyosNetworkLayerC.nc"
static inline uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__payloadLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__payloadLength(msg) - /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__PAYLOAD_OFFSET;
}

# 49 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/RadioPacket.nc"
inline static uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__payloadLength(message_t *msg){
#line 49
  unsigned char __nesc_result;
#line 49

#line 49
  __nesc_result = /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__payloadLength(msg);
#line 49

#line 49
  return __nesc_result;
#line 49
}
#line 49
# 230 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/ActiveMessageLayerP.nc"
static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__payloadLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__payloadLength(msg) - sizeof(activemessage_header_t );
}

#line 262
static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Packet__payloadLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__payloadLength(msg);
}

#line 225
static inline uint8_t /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__headerLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubPacket__headerLength(msg) + sizeof(activemessage_header_t );
}

#line 72
static inline void */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getPayload(message_t *msg)
{
  return (void *)msg + /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__RadioPacket__headerLength(msg);
}

#line 128
static inline message_t */*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubReceive__receive(message_t *msg)
{
  am_id_t id = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__type(msg);
  void *payload = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__getPayload(msg);
  uint8_t len = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Packet__payloadLength(msg);

  msg = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__AMPacket__isForMe(msg) ? 
  /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Receive__receive(id, msg, payload, len) : 
  /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__Snoop__receive(id, msg, payload, len);

  return msg;
}

# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
inline static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosReceive__receive(message_t *msg){
#line 42
  nx_struct message_t *__nesc_result;
#line 42

#line 42
  __nesc_result = /*CC2520RpiAmRadioC.ActiveMessageLayerC.ActiveMessageLayerP*/ActiveMessageLayerP__0__SubReceive__receive(msg);
#line 42

#line 42
  return __nesc_result;
#line 42
}
#line 42
# 10 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAm154DummyP.nc"
static inline message_t *CC2520RpiAm154DummyP__Ieee154Receive__receive(message_t *msg)
#line 10
{
#line 10
  return msg;
}

# 42 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/util/BareReceive.nc"
inline static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Receive__receive(message_t *msg){
#line 42
  nx_struct message_t *__nesc_result;
#line 42

#line 42
  __nesc_result = CC2520RpiAm154DummyP__Ieee154Receive__receive(msg);
#line 42

#line 42
  return __nesc_result;
#line 42
}
#line 42
# 69 "/home/wwhuang/git/tinyos-lab11/tos/types/TinyError.h"
static inline  error_t ecombine(error_t r1, error_t r2)




{
  return r1 == r2 ? r1 : FAIL;
}

# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
inline static error_t CC2520RpiReceiveP__IO__registerFileDescriptor(int file_descriptor){
#line 4
  unsigned char __nesc_result;
#line 4

#line 4
  __nesc_result = IOManagerP__IO__registerFileDescriptor(0U, file_descriptor);
#line 4

#line 4
  return __nesc_result;
#line 4
}
#line 4
# 143 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc"
static inline error_t CC2520RpiReceiveP__SoftwareInit__init(void )
#line 143
{


  int cc2520_file;
  int ret;

  CC2520RpiReceiveP__rx_msg_ptr = (uint8_t *)&CC2520RpiReceiveP__rx_msg_buf;

  cc2520_file = open("/dev/radio", 02);
  if (cc2520_file < 0) {
      do {
#line 153
          flockfile(stderr);
#line 153
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 153);
#line 153
          fprintf(stderr, "Could not open radio.\n");
#line 153
          funlockfile(stderr);
        }
      while (
#line 153
      0);
      exit(1);
    }


  ret = pipe(CC2520RpiReceiveP__read_pipe);
  if (ret == -1) {
      do {
#line 160
          flockfile(stderr);
#line 160
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 160);
#line 160
          fprintf(stderr, "Could not create pipe.\n");
#line 160
          funlockfile(stderr);
        }
      while (
#line 160
      0);
      exit(1);
    }



  if (!fork()) {

      uint8_t pkt_buf[256];

#line 169
      close(CC2520RpiReceiveP__read_pipe[0]);

      {

        const char RX_STR[9] = "-2520-Rx";
        char proc_name[17] = { 0 };

#line 175
        prctl(16, proc_name, 0, 0, 0);
        if (strlen(proc_name) > 16 - strlen(RX_STR)) {
            strcpy(proc_name + 16 - strlen(RX_STR), RX_STR);
          }
        else 
#line 178
          {
            strcat(proc_name, RX_STR);
          }
        prctl(15, proc_name, 0, 0, 0);

        ;
      }



      ret = prctl(1, 9);
      if (ret == -1) {
          do {
#line 190
              flockfile(stderr);
#line 190
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 190);
#line 190
              fprintf(stderr, "Error setting signal when the parent dies.\n");
#line 190
              funlockfile(stderr);
            }
          while (
#line 190
          0);
          do {
#line 191
              flockfile(stderr);
#line 191
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 191);
#line 191
              fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 191
              funlockfile(stderr);
            }
          while (
#line 191
          0);
        }

      while (1) {
          ssize_t len;

#line 196
          len = read(cc2520_file, pkt_buf, 256);
          if (len <= 0) {
              do {
#line 198
                  flockfile(stderr);
#line 198
                  fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 198);
#line 198
                  fprintf(stderr, "Pipe died.\n");
#line 198
                  funlockfile(stderr);
                }
              while (
#line 198
              0);
              close(CC2520RpiReceiveP__read_pipe[1]);
            }
          write(CC2520RpiReceiveP__read_pipe[1], pkt_buf, len);
        }
    }


  close(CC2520RpiReceiveP__read_pipe[1]);
  close(cc2520_file);

  CC2520RpiReceiveP__cc2520_pipe = CC2520RpiReceiveP__read_pipe[0];


  make_nonblocking(CC2520RpiReceiveP__cc2520_pipe);


  CC2520RpiReceiveP__IO__registerFileDescriptor(CC2520RpiReceiveP__cc2520_pipe);

  ;

  return SUCCESS;
}

# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
inline static error_t CC2520RpiSendP__IO__registerFileDescriptor(int file_descriptor){
#line 4
  unsigned char __nesc_result;
#line 4

#line 4
  __nesc_result = IOManagerP__IO__registerFileDescriptor(1U, file_descriptor);
#line 4

#line 4
  return __nesc_result;
#line 4
}
#line 4
# 80 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc"
static inline error_t CC2520RpiSendP__SoftwareInit__init(void )
#line 80
{
  int cc2520_file;
  int ret;


  cc2520_file = open("/dev/radio", 02);
  if (cc2520_file < 0) {
      do {
#line 87
          flockfile(stderr);
#line 87
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 87);
#line 87
          fprintf(stderr, "Could not open radio.\n");
#line 87
          funlockfile(stderr);
        }
      while (
#line 87
      0);
      exit(1);
    }


  ret = pipe(CC2520RpiSendP__write_pipe);
  if (ret == -1) {
      do {
#line 94
          flockfile(stderr);
#line 94
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 94);
#line 94
          fprintf(stderr, "Could not create write pipe.\n");
#line 94
          funlockfile(stderr);
        }
      while (
#line 94
      0);
      exit(1);
    }


  ret = pipe(CC2520RpiSendP__read_pipe);
  if (ret == -1) {
      do {
#line 101
          flockfile(stderr);
#line 101
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 101);
#line 101
          fprintf(stderr, "Could not create read pipe.\n");
#line 101
          funlockfile(stderr);
        }
      while (
#line 101
      0);
      exit(1);
    }



  if (!fork()) {

      CC2520RpiSendP__write_fifo_header_t whdr;
      CC2520RpiSendP__read_fifo_header_t rhdr;
      uint8_t pkt_buf[256];

#line 112
      close(CC2520RpiSendP__read_pipe[0]);
      close(CC2520RpiSendP__write_pipe[1]);

      {
        const char RX_STR[9] = "-2520-Tx";
        char proc_name[17] = { 0 };

#line 118
        prctl(16, proc_name, 0, 0, 0);
        if (strlen(proc_name) > 16 - strlen(RX_STR)) {
            strcpy(proc_name + 16 - strlen(RX_STR), RX_STR);
          }
        else 
#line 121
          {
            strcat(proc_name, RX_STR);
          }
        prctl(15, proc_name, 0, 0, 0);

        ;
      }



      ret = prctl(1, 9);
      if (ret == -1) {
          do {
#line 133
              flockfile(stderr);
#line 133
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 133);
#line 133
              fprintf(stderr, "Error setting signal when the parent dies.\n");
#line 133
              funlockfile(stderr);
            }
          while (
#line 133
          0);
          do {
#line 134
              flockfile(stderr);
#line 134
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 134);
#line 134
              fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 134
              funlockfile(stderr);
            }
          while (
#line 134
          0);
        }

      while (1) {
          ssize_t len;
#line 138
          ssize_t ret_val;

#line 139
          len = read(CC2520RpiSendP__write_pipe[0], &whdr, sizeof(CC2520RpiSendP__write_fifo_header_t ));
          if (len == 0) {
              do {
#line 141
                  flockfile(stderr);
#line 141
                  fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 141);
#line 141
                  fprintf(stderr, "Write pipe EOF.\n");
#line 141
                  funlockfile(stderr);
                }
              while (
#line 141
              0);
            }
          else {
#line 142
            if (len < 0) {
                do {
#line 143
                    flockfile(stderr);
#line 143
                    fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 143);
#line 143
                    fprintf(stderr, "Pipe error: %i.\n", *__errno_location());
#line 143
                    funlockfile(stderr);
                  }
                while (
#line 143
                0);
                close(CC2520RpiSendP__read_pipe[1]);
                close(CC2520RpiSendP__write_pipe[0]);
                exit(1);
              }
            }

          pkt_buf[0] = whdr.len;



          len = read(CC2520RpiSendP__write_pipe[0], pkt_buf + 1, whdr.len - 2);
          if (len <= 0) {
              do {
#line 156
                  flockfile(stderr);
#line 156
                  fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 156);
#line 156
                  fprintf(stderr, "Error reading from pipe.\n");
#line 156
                  funlockfile(stderr);
                }
              while (
#line 156
              0);
              close(CC2520RpiSendP__read_pipe[1]);
              close(CC2520RpiSendP__write_pipe[0]);
            }



          ret_val = write(cc2520_file, pkt_buf, whdr.len - 1);


          rhdr.ptr_to_msg = whdr.ptr_to_msg;
          rhdr.ret = ret_val;
          rhdr.len = whdr.len;
          ret_val = write(CC2520RpiSendP__read_pipe[1], &rhdr, sizeof(CC2520RpiSendP__read_fifo_header_t ));
          if (ret_val == -1) {
              do {
#line 171
                  flockfile(stderr);
#line 171
                  fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 171);
#line 171
                  fprintf(stderr, "Error writing to read pipe.\n");
#line 171
                  funlockfile(stderr);
                }
              while (
#line 171
              0);
            }
          else {
#line 172
            if (ret_val != sizeof(CC2520RpiSendP__read_fifo_header_t )) {
                do {
#line 173
                    flockfile(stderr);
#line 173
                    fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 173);
#line 173
                    fprintf(stderr, "Return code was not fully written to the pipe\n");
#line 173
                    funlockfile(stderr);
                  }
                while (
#line 173
                0);
                do {
#line 174
                    flockfile(stderr);
#line 174
                    fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 174);
#line 174
                    fprintf(stderr, "Only %i bytes were written\n", ret_val);
#line 174
                    funlockfile(stderr);
                  }
                while (
#line 174
                0);
              }
            }
        }
    }

  close(CC2520RpiSendP__read_pipe[1]);
  close(CC2520RpiSendP__write_pipe[0]);
  close(cc2520_file);

  CC2520RpiSendP__cc2520_read = CC2520RpiSendP__read_pipe[0];
  CC2520RpiSendP__cc2520_write = CC2520RpiSendP__write_pipe[1];




  make_nonblocking(CC2520RpiSendP__cc2520_read);


  CC2520RpiSendP__IO__registerFileDescriptor(CC2520RpiSendP__cc2520_read);

  ;

  return SUCCESS;
}

# 18 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpiAM/CC2520RpiAmUniqueP.nc"
static inline error_t CC2520RpiAmUniqueP__Init__init(void )
#line 18
{
  CC2520RpiAmUniqueP__seq_no = TOS_NODE_ID << 4;
  return SUCCESS;
}

# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
inline static error_t /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__postTask(void ){
#line 67
  unsigned char __nesc_result;
#line 67

#line 67
  __nesc_result = SchedulerBasicP__TaskBasic__postTask(/*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired);
#line 67

#line 67
  return __nesc_result;
#line 67
}
#line 67
# 82 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__fired(void )
#line 82
{
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__postTask();
}

# 78 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/Alarm.nc"
inline static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__fired(void ){
#line 78
  /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__Alarm__fired();
#line 78
}
#line 78
# 19 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
static inline void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__AlarmMilli32Fired(int sig)
#line 19
{
  /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__fired();
}

static inline error_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Init__init(void )
#line 23
{
  int t_ret;



  t_ret = timer_create(1, (void *)0, &/*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__timerid);


  signal(14, /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__AlarmMilli32Fired);

  /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__last_alarm = 0;

  if (t_ret == 0) {
      return SUCCESS;
    }
  else 
#line 37
    {
      return FAIL;
    }
}

# 62 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Init.nc"
inline static error_t RealMainP__SoftwareInit__init(void ){
#line 62
  unsigned char __nesc_result;
#line 62

#line 62
  __nesc_result = /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Init__init();
#line 62
  __nesc_result = ecombine(__nesc_result, CC2520RpiAmUniqueP__Init__init());
#line 62
  __nesc_result = ecombine(__nesc_result, CC2520RpiSendP__SoftwareInit__init());
#line 62
  __nesc_result = ecombine(__nesc_result, CC2520RpiReceiveP__SoftwareInit__init());
#line 62

#line 62
  return __nesc_result;
#line 62
}
#line 62
# 137 "OpoBaseP.nc"
static inline void OpoBaseP__RfControl__startDone(error_t err)
#line 137
{
  printf("RfStartDone\n");
  printfflush();
}

# 113 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
inline static void CC2520RpiRadioP__SplitControl__startDone(error_t error){
#line 113
  OpoBaseP__RfControl__startDone(error);
#line 113
}
#line 113
# 24 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411P.nc"
static inline bool Ds2411P__ds2411_check_crc(const ds2411_serial_t *id)
#line 24
{
  uint8_t crc = 0;
  uint8_t idx;

#line 27
  for (idx = 0; idx < DS2411_DATA_LENGTH; idx++) {
      uint8_t i;

#line 29
      crc = crc ^ id->data[idx];
      for (i = 0; i < 8; i++) {
          if (crc & 0x01) {
              crc = (crc >> 1) ^ 0x8C;
            }
          else 
#line 33
            {
              crc >>= 1;
            }
        }
    }
  return crc == 0;
}

# 23 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411PowerControlC.nc"
static inline error_t Ds2411PowerControlC__StdControl__stop(void )
#line 23
{
  return SUCCESS;
}

# 105 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/StdControl.nc"
inline static error_t Ds2411P__PowerControl__stop(void ){
#line 105
  unsigned char __nesc_result;
#line 105

#line 105
  __nesc_result = Ds2411PowerControlC__StdControl__stop();
#line 105

#line 105
  return __nesc_result;
#line 105
}
#line 105
# 66 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/BusyWait.nc"
inline static void OneWireMasterP__BusyWait__wait(OneWireMasterP__BusyWait__size_type dt){
#line 66
  BusyWaitMicroP__BusyWait__wait(dt);
#line 66
}
#line 66
# 42 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static bool /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__get(void ){
#line 42
  unsigned char __nesc_result;
#line 42

#line 42
  __nesc_result = HplBcm2835GeneralIOP__Gpio__get(RPI_V2_GPIO_P1_08);
#line 42

#line 42
  return __nesc_result;
#line 42
}
#line 42
# 18 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline bool /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__get(void )
#line 18
{
#line 18
  return /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__get();
}

# 43 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static bool OneWireMasterP__Pin__get(void ){
#line 43
  unsigned char __nesc_result;
#line 43

#line 43
  __nesc_result = /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__get();
#line 43

#line 43
  return __nesc_result;
#line 43
}
#line 43
# 47 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeInput(void ){
#line 47
  HplBcm2835GeneralIOP__Gpio__makeInput(RPI_V2_GPIO_P1_08);
#line 47
}
#line 47
# 19 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeInput(void )
#line 19
{
#line 19
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeInput();
}

# 44 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void OneWireMasterP__Pin__makeInput(void ){
#line 44
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeInput();
#line 44
}
#line 44
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeOutput(void ){
#line 54
  HplBcm2835GeneralIOP__Gpio__makeOutput(RPI_V2_GPIO_P1_08);
#line 54
}
#line 54
# 21 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeOutput(void )
#line 21
{
#line 21
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__makeOutput();
}

# 46 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void OneWireMasterP__Pin__makeOutput(void ){
#line 46
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__makeOutput();
#line 46
}
#line 46
# 66 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireMasterP.nc"
static inline bool OneWireMasterP__readBit(void )
#line 66
{
  bool bit;

#line 68
  OneWireMasterP__Pin__makeOutput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_5US);
  OneWireMasterP__Pin__makeInput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_5US);
  bit = OneWireMasterP__Pin__get();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__SLOT_TIME);
  return bit;
}

#line 89
static inline uint8_t OneWireMasterP__readByte(void )
#line 89
{
  uint8_t i;
#line 90
  uint8_t c = 0;

#line 91
  for (i = 0; i < 8; i++) {
      c >>= 1;
      if (OneWireMasterP__readBit()) {
          c |= 0x80;
        }
    }
  return c;
}

#line 59
static inline void OneWireMasterP__writeZero(void )
#line 59
{
  OneWireMasterP__Pin__makeOutput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_60US);
  OneWireMasterP__Pin__makeInput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_5US);
}

#line 52
static inline void OneWireMasterP__writeOne(void )
#line 52
{
  OneWireMasterP__Pin__makeOutput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_5US);
  OneWireMasterP__Pin__makeInput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__SLOT_TIME);
}

#line 77
static inline void OneWireMasterP__writeByte(uint8_t c)
#line 77
{
  uint8_t j;

#line 79
  for (j = 0; j < 8; j++) {
      if (c & 0x01) {
          OneWireMasterP__writeOne();
        }
      else 
#line 82
        {
          OneWireMasterP__writeZero();
        }
      c >>= 1;
    }
}

# 83 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
static inline void HplBcm2835GeneralIOP__Gpio__clr(uint8_t bcm_pin)
#line 83
{
  *(HplBcm2835GeneralIOP__gpio + 10) = 1 << bcm_pin;
  /* atomic removed: atomic calls only */
#line 85
  HplBcm2835GeneralIOP__pin_level &= ~(1 << bcm_pin);
}

# 22 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIO.nc"
inline static void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__clr(void ){
#line 22
  HplBcm2835GeneralIOP__Gpio__clr(RPI_V2_GPIO_P1_08);
#line 22
}
#line 22
# 16 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/Bcm2835GpioC.nc"
static inline void /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__clr(void )
#line 16
{
#line 16
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__IO__clr();
}

# 41 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/GeneralIO.nc"
inline static void OneWireMasterP__Pin__clr(void ){
#line 41
  /*HplDs2411C.BcmGpio*/Bcm2835GpioC__3__GeneralIO__clr();
#line 41
}
#line 41
# 34 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireMasterP.nc"
static inline bool OneWireMasterP__reset(void )
#line 34
{
  uint16_t i;

#line 36
  OneWireMasterP__Pin__makeInput();
  OneWireMasterP__Pin__clr();
  OneWireMasterP__Pin__makeOutput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__RESET_LOW_TIME);
  OneWireMasterP__Pin__makeInput();
  OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_60US);

  for (i = 0; 
  i < OneWireMasterP__PRESENCE_DETECT_LOW_TIME; 
  i += OneWireMasterP__DELAY_5US, OneWireMasterP__BusyWait__wait(OneWireMasterP__DELAY_5US)) {
      if (!OneWireMasterP__Pin__get()) {
#line 46
        break;
        }
    }
#line 48
  OneWireMasterP__BusyWait__wait(OneWireMasterP__PRESENCE_RESET_HIGH_TIME - OneWireMasterP__DELAY_60US);
  return i < OneWireMasterP__PRESENCE_DETECT_LOW_TIME;
}

#line 100
static inline error_t OneWireMasterP__OneWire__read(uint8_t cmd, uint8_t *buf, uint8_t len)
#line 100
{
  error_t e = SUCCESS;

#line 102
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 102
    {
      if (OneWireMasterP__reset()) {
          uint8_t i;

#line 105
          OneWireMasterP__writeByte(cmd);
          for (i = 0; i < len; i++) {
              buf[i] = OneWireMasterP__readByte();
            }
        }
      else 
#line 109
        {
          e = EOFF;
        }
    }
#line 112
    __nesc_atomic_end(__nesc_atomic); }
  return e;
}

# 10 "/home/wwhuang/git/tinyos-lab11/tos/lib/onewire/OneWireReadWrite.nc"
inline static error_t Ds2411P__OneWire__read(uint8_t cmd, uint8_t *buf, uint8_t len){
#line 10
  unsigned char __nesc_result;
#line 10

#line 10
  __nesc_result = OneWireMasterP__OneWire__read(cmd, buf, len);
#line 10

#line 10
  return __nesc_result;
#line 10
}
#line 10
# 19 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411PowerControlC.nc"
static inline error_t Ds2411PowerControlC__StdControl__start(void )
#line 19
{
  return SUCCESS;
}

# 95 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/StdControl.nc"
inline static error_t Ds2411P__PowerControl__start(void ){
#line 95
  unsigned char __nesc_result;
#line 95

#line 95
  __nesc_result = Ds2411PowerControlC__StdControl__start();
#line 95

#line 95
  return __nesc_result;
#line 95
}
#line 95
# 41 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/Ds2411P.nc"
static inline error_t Ds2411P__readId(void )
#line 41
{
  error_t e;

  e = Ds2411P__PowerControl__start();
  if (e != SUCCESS) {
#line 45
    return FAIL;
    }
  e = Ds2411P__OneWire__read(0x33, 
  Ds2411P__ds2411id.data, 
  DS2411_DATA_LENGTH);
  Ds2411P__PowerControl__stop();

  if (e == SUCCESS) {
      if (Ds2411P__ds2411_check_crc(&Ds2411P__ds2411id)) {
          uint16_t id_sum = 0;
          uint8_t i = 0;

#line 56
          for (i = 0; i < DS2411_SERIAL_LENGTH; i++) {
              id_sum += Ds2411P__ds2411id.serial[i];
            }
          if (id_sum != 0) {
              Ds2411P__haveId = TRUE;
            }
        }
      else 
#line 62
        {
          e = 22;
        }
    }
  return e;
}

static inline error_t Ds2411P__ReadId48__read(uint8_t *id)
#line 69
{
  error_t e = SUCCESS;

#line 71
  if (!Ds2411P__haveId) {
      e = Ds2411P__readId();
    }
  if (Ds2411P__haveId) {
      memcpy(id, Ds2411P__ds2411id.serial, DS2411_SERIAL_LENGTH);
    }
  return e;
}

# 13 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/ReadId48.nc"
inline static error_t LocalIeeeEui64P__ReadId48__read(uint8_t *id){
#line 13
  unsigned char __nesc_result;
#line 13

#line 13
  __nesc_result = Ds2411P__ReadId48__read(id);
#line 13

#line 13
  return __nesc_result;
#line 13
}
#line 13
# 17 "/home/wwhuang/git/tinyos-lab11/tos/chips/ds2411/LocalIeeeEui64P.nc"
static inline ieee_eui64_t LocalIeeeEui64P__LocalIeeeEui64__getId(void )
#line 17
{
  uint8_t buf[6] = { 0 };
  error_t e;

  if (!LocalIeeeEui64P__have_id) {
      e = LocalIeeeEui64P__ReadId48__read(buf);
      if (e == SUCCESS) {
          LocalIeeeEui64P__eui.data[0] = IEEE_EUI64_COMPANY_ID_0;
          LocalIeeeEui64P__eui.data[1] = IEEE_EUI64_COMPANY_ID_1;
          LocalIeeeEui64P__eui.data[2] = IEEE_EUI64_COMPANY_ID_2;



          LocalIeeeEui64P__eui.data[3] = IEEE_EUI64_SERIAL_ID_0;
          LocalIeeeEui64P__eui.data[4] = IEEE_EUI64_SERIAL_ID_1;


          LocalIeeeEui64P__eui.data[5] = buf[2];
          LocalIeeeEui64P__eui.data[6] = buf[1];
          LocalIeeeEui64P__eui.data[7] = buf[0];

          LocalIeeeEui64P__have_id = TRUE;
        }
    }
  return LocalIeeeEui64P__eui;
}

# 48 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/LocalIeeeEui64.nc"
inline static ieee_eui64_t CC2520RpiRadioP__LocalIeeeEui64__getId(void ){
#line 48
  struct ieee_eui64 __nesc_result;
#line 48

#line 48
  __nesc_result = LocalIeeeEui64P__LocalIeeeEui64__getId();
#line 48

#line 48
  return __nesc_result;
#line 48
}
#line 48
# 54 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc"
static inline error_t CC2520RpiRadioP__SplitControl__start(void )
#line 54
{
  uint8_t *ext_addr_ptr;
  int i;
  uint16_t sum = 0;

  ;
  CC2520RpiRadioP__cc2520_file = open("/dev/radio", 02);
  if (CC2520RpiRadioP__cc2520_file < 0) {
      do {
#line 62
          flockfile(stderr);
#line 62
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc", 62);
#line 62
          fprintf(stderr, "Failed to open /dev/radio.\n");
#line 62
          funlockfile(stderr);
        }
      while (
#line 62
      0);
      do {
#line 63
          flockfile(stderr);
#line 63
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiRadioP.nc", 63);
#line 63
          fprintf(stderr, "Make sure the kernel module is loaded.\n");
#line 63
          funlockfile(stderr);
        }
      while (
#line 63
      0);
      exit(1);
    }


  CC2520RpiRadioP__sequence_number = TOS_NODE_ID << 3;


  CC2520RpiRadioP__addr_data.short_addr = TOS_NODE_ID;
  do {

      CC2520RpiRadioP__ext_addr = CC2520RpiRadioP__LocalIeeeEui64__getId();
      for (i = 0; i < 8; i++) {
          sum += CC2520RpiRadioP__ext_addr.data[i];
        }
    }
  while (
#line 78
  sum == 0);
  ext_addr_ptr = (uint8_t *)& CC2520RpiRadioP__addr_data.extended_addr;
  for (i = 0; i < 8; i++) {
      ext_addr_ptr[i] = CC2520RpiRadioP__ext_addr.data[7 - i];
    }


  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (3 << 0)) | (sizeof(struct cc2520_set_channel_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__chan_data);
  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (4 << 0)) | (sizeof(struct cc2520_set_address_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__addr_data);
  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (6 << 0)) | (sizeof(struct cc2520_set_ack_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__ack_data);
  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (5 << 0)) | (sizeof(struct cc2520_set_txpower_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__txpower_data);
  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (7 << 0)) | (sizeof(struct cc2520_set_lpl_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__lpl_data);
  ioctl(CC2520RpiRadioP__cc2520_file, (((1U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (9 << 0)) | (sizeof(struct cc2520_set_print_messages_data ) << (0 + 8 + 8)), &CC2520RpiRadioP__print_data);


  ioctl(CC2520RpiRadioP__cc2520_file, (((0U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (0 << 0)) | (0 << (0 + 8 + 8)), (void *)0);
  ioctl(CC2520RpiRadioP__cc2520_file, (((0U << (0 + 8 + 8 + 14)) | (0xCC << (0 + 8))) | (1 << 0)) | (0 << (0 + 8 + 8)), (void *)0);

  CC2520RpiRadioP__SplitControl__startDone(SUCCESS);
  return SUCCESS;
}

# 104 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/SplitControl.nc"
inline static error_t OpoBaseP__RfControl__start(void ){
#line 104
  unsigned char __nesc_result;
#line 104

#line 104
  __nesc_result = CC2520RpiRadioP__SplitControl__start();
#line 104

#line 104
  return __nesc_result;
#line 104
}
#line 104
# 20 "OpoBaseP.nc"
static inline void OpoBaseP__Boot__booted(void )
#line 20
{
  OpoBaseP__RfControl__start();
}

# 60 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Boot.nc"
inline static void RealMainP__Boot__booted(void ){
#line 60
  OpoBaseP__Boot__booted();
#line 60
}
#line 60
# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
inline static error_t CC2520RpiReceiveP__receive_task__postTask(void ){
#line 67
  unsigned char __nesc_result;
#line 67

#line 67
  __nesc_result = SchedulerBasicP__TaskBasic__postTask(CC2520RpiReceiveP__receive_task);
#line 67

#line 67
  return __nesc_result;
#line 67
}
#line 67
# 101 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc"
static inline void CC2520RpiReceiveP__IO__receiveReady(void )
#line 101
{
  ssize_t ret;



  ret = read(CC2520RpiReceiveP__cc2520_pipe, CC2520RpiReceiveP__rx_msg_ptr, 1);
  if (ret == 0) {

      ;
      return;
    }
  else {
#line 112
    if (ret == -1) {
        switch (*__errno_location()) {
            case 11: 




              ;
            return;
            default: 
              do {
#line 122
                  flockfile(stderr);
#line 122
                  fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 122);
#line 122
                  fprintf(stderr, "Error reading length from cc2520. errno: %i\n", *__errno_location());
#line 122
                  funlockfile(stderr);
                }
              while (
#line 122
              0);
            do {
#line 123
                flockfile(stderr);
#line 123
                fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 123);
#line 123
                fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 123
                funlockfile(stderr);
              }
            while (
#line 123
            0);
            exit(1);
          }
      }
    }

  ret = read(CC2520RpiReceiveP__cc2520_pipe, CC2520RpiReceiveP__rx_msg_ptr + 1, CC2520RpiReceiveP__rx_msg_ptr[0]);
  if (ret == -1) {
      do {
#line 131
          flockfile(stderr);
#line 131
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 131);
#line 131
          fprintf(stderr, "Error reading packet from cc2520. errno: %i\n", *__errno_location());
#line 131
          funlockfile(stderr);
        }
      while (
#line 131
      0);
      do {
#line 132
          flockfile(stderr);
#line 132
          fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiReceiveP.nc", 132);
#line 132
          fprintf(stderr, "%s", strerror(*__errno_location()));
#line 132
          funlockfile(stderr);
        }
      while (
#line 132
      0);
      exit(1);
    }
  else {
#line 134
    if (ret != CC2520RpiReceiveP__rx_msg_ptr[0]) {


        return;
      }
    }
  CC2520RpiReceiveP__receive_task__postTask();
}

# 67 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
inline static error_t CC2520RpiSendP__sendDone_task__postTask(void ){
#line 67
  unsigned char __nesc_result;
#line 67

#line 67
  __nesc_result = SchedulerBasicP__TaskBasic__postTask(CC2520RpiSendP__sendDone_task);
#line 67

#line 67
  return __nesc_result;
#line 67
}
#line 67
# 201 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc"
static inline void CC2520RpiSendP__IO__receiveReady(void )
#line 201
{
  ssize_t ret;

  ;

  ret = read(CC2520RpiSendP__cc2520_read, &CC2520RpiSendP__send_hdr, sizeof(CC2520RpiSendP__read_fifo_header_t ));
  if (ret == -1) {
      switch (*__errno_location()) {
          case 11: 




            ;
          return;
          default: 
            do {
#line 217
                flockfile(stderr);
#line 217
                fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 217);
#line 217
                fprintf(stderr, "Error with packet result fifo. errno: %i\n", *__errno_location());
#line 217
                funlockfile(stderr);
              }
            while (
#line 217
            0);
          do {
#line 218
              flockfile(stderr);
#line 218
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 218);
#line 218
              fprintf(stderr, "%s\n", strerror(*__errno_location()));
#line 218
              funlockfile(stderr);
            }
          while (
#line 218
          0);
          exit(1);
        }
    }
  else {
#line 221
    if (ret != sizeof(CC2520RpiSendP__read_fifo_header_t )) {

        do {
#line 223
            flockfile(stderr);
#line 223
            fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/cc2520rpi/CC2520RpiSendP.nc", 223);
#line 223
            fprintf(stderr, "Read only %i bytes from the packet result fifo\n", ret);
#line 223
            funlockfile(stderr);
          }
        while (
#line 223
        0);




        return;
      }
    }

  CC2520RpiSendP__sendDone_task__postTask();
}

# 92 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
static inline void IOManagerP__IO__default__receiveReady(uint8_t id)
#line 92
{
}

# 9 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/IO.nc"
inline static void IOManagerP__IO__receiveReady(uint8_t arg_0x2ab95b9ae850){
#line 9
  switch (arg_0x2ab95b9ae850) {
#line 9
    case 0U:
#line 9
      CC2520RpiReceiveP__IO__receiveReady();
#line 9
      break;
#line 9
    case 1U:
#line 9
      CC2520RpiSendP__IO__receiveReady();
#line 9
      break;
#line 9
    default:
#line 9
      IOManagerP__IO__default__receiveReady(arg_0x2ab95b9ae850);
#line 9
      break;
#line 9
    }
#line 9
}
#line 9
# 47 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
static inline void IOManagerP__BlockingIO__waitForIO(void )
#line 47
{
  int ret;
  int i;
  uint8_t nfds = 0;


  do {
#line 53
      unsigned int __i;
#line 53
      fd_set *__arr = &IOManagerP__rfds;

#line 53
      for (__i = 0; __i < sizeof(fd_set ) / sizeof(__fd_mask ); ++__i) __arr->__fds_bits[__i] = 0;
    }
  while (
#line 53
  0);
  for (i = 0; i < IOManagerP__N_FDS; i++) {
      if (IOManagerP__map[i] != 1) {
          (void )((&IOManagerP__rfds)->__fds_bits[IOManagerP__map[i] / (8 * (int )sizeof(__fd_mask ))] |= (__fd_mask )1 << IOManagerP__map[i] % (8 * (int )sizeof(__fd_mask )));
          if (IOManagerP__map[i] + 1 > nfds) {
              nfds = IOManagerP__map[i] + 1;
            }
        }
    }


  ret = select(nfds, &IOManagerP__rfds, (void *)0, (void *)0, (void *)0);

  if (ret < 0) {
      if (*__errno_location() == 4) {
        }
      else {

          do {
#line 71
              flockfile(stderr);
#line 71
              fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc", 71);
#line 71
              fprintf(stderr, "select return error: %i\n", ret);
#line 71
              funlockfile(stderr);
            }
          while (
#line 71
          0);
        }
    }
  else {
#line 74
    if (ret == 0) {
        do {
#line 75
            flockfile(stderr);
#line 75
            fprintf(stderr, "%s:%d\t", "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc", 75);
#line 75
            fprintf(stderr, "select return 0.\n");
#line 75
            funlockfile(stderr);
          }
        while (
#line 75
        0);
      }
    else {

        int j;

        if (ret > 1) {
            ;
          }
        for (j = 0; j < IOManagerP__N_FDS; j++) {
            if (((&IOManagerP__rfds)->__fds_bits[IOManagerP__map[j] / (8 * (int )sizeof(__fd_mask ))] & ((__fd_mask )1 << IOManagerP__map[j] % (8 * (int )sizeof(__fd_mask )))) != 0) {
                IOManagerP__IO__receiveReady(j);
              }
          }
      }
    }
}

# 4 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/interfaces/BlockingIO.nc"
inline static void McuSleepP__BlockingIO__waitForIO(void ){
#line 4
  IOManagerP__BlockingIO__waitForIO();
#line 4
}
#line 4
# 36 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/linux_atomic.h"
static inline  void __nesc_enable_interrupt(void )
#line 36
{


  interupts_enabled = TRUE;
  sigprocmask(2, &global_sigmask, (void *)0);
}

# 20 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/McuSleepP.nc"
static inline void McuSleepP__McuSleep__sleep(void )
#line 20
{
  __nesc_enable_interrupt();
  McuSleepP__BlockingIO__waitForIO();
  __nesc_disable_interrupt();
}

# 76 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/McuSleep.nc"
inline static void SchedulerBasicP__McuSleep__sleep(void ){
#line 76
  McuSleepP__McuSleep__sleep();
#line 76
}
#line 76
# 78 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
static __inline uint8_t SchedulerBasicP__popTask(void )
{
  if (SchedulerBasicP__m_head != SchedulerBasicP__NO_TASK) 
    {
      uint8_t id = SchedulerBasicP__m_head;

#line 83
      SchedulerBasicP__m_head = SchedulerBasicP__m_next[SchedulerBasicP__m_head];
      if (SchedulerBasicP__m_head == SchedulerBasicP__NO_TASK) 
        {
          SchedulerBasicP__m_tail = SchedulerBasicP__NO_TASK;
        }
      SchedulerBasicP__m_next[id] = SchedulerBasicP__NO_TASK;
      return id;
    }
  else 
    {
      return SchedulerBasicP__NO_TASK;
    }
}

#line 149
static inline void SchedulerBasicP__Scheduler__taskLoop(void )
{
  for (; ; ) 
    {
      uint8_t nextTask;

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
        {
          while ((nextTask = SchedulerBasicP__popTask()) == SchedulerBasicP__NO_TASK) 
            {
              SchedulerBasicP__McuSleep__sleep();
            }
        }
#line 161
        __nesc_atomic_end(__nesc_atomic); }
      SchedulerBasicP__TaskBasic__runTask(nextTask);
    }
}

# 72 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/Scheduler.nc"
inline static void RealMainP__Scheduler__taskLoop(void ){
#line 72
  SchedulerBasicP__Scheduler__taskLoop();
#line 72
}
#line 72
# 43 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/linux_atomic.h"
  __nesc_atomic_t __nesc_atomic_start(void )
#line 43
{
  __nesc_atomic_t prev_state = interupts_enabled;

#line 45
  __nesc_disable_interrupt();
  return prev_state;
}

#line 17
static  void __nesc_disable_interrupt(void )
#line 17
{
  sigset_t all;
  sigset_t temp_sigmask;




  sigfillset(&all);
  sigprocmask(0, &all, &temp_sigmask);




  if (interupts_enabled) {
      global_sigmask = temp_sigmask;
      interupts_enabled = FALSE;
    }
}

#line 49
  void __nesc_atomic_end(__nesc_atomic_t reenable_interrupts)
#line 49
{
  if (reenable_interrupts) {
      __nesc_enable_interrupt();
    }
}

# 63 "/home/wwhuang/git/tinyos-lab11/tos/system/RealMainP.nc"
  int main(void )
#line 63
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {





      {
      }
#line 71
      ;

      RealMainP__Scheduler__init();





      RealMainP__PlatformInit__init();
      while (RealMainP__Scheduler__runNextTask()) ;





      RealMainP__SoftwareInit__init();
      while (RealMainP__Scheduler__runNextTask()) ;
    }
#line 88
    __nesc_atomic_end(__nesc_atomic); }


  __nesc_enable_interrupt();

  RealMainP__Boot__booted();


  RealMainP__Scheduler__taskLoop();




  return -1;
}

# 117 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
static void HplBcm2835GeneralIOP__Gpio__makeOutput(uint8_t bcm_pin)
#line 117
{
  *(HplBcm2835GeneralIOP__gpio + bcm_pin / 10) &= ~(7 << bcm_pin % 10 * 3);
  *(HplBcm2835GeneralIOP__gpio + bcm_pin / 10) |= 1 << bcm_pin % 10 * 3;
  /* atomic removed: atomic calls only */
#line 120
  HplBcm2835GeneralIOP__pin_output |= 1 << bcm_pin;
}

#line 78
static void HplBcm2835GeneralIOP__Gpio__set(uint8_t bcm_pin)
#line 78
{
  *(HplBcm2835GeneralIOP__gpio + 7) = 1 << bcm_pin;
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 80
    HplBcm2835GeneralIOP__pin_level |= 1 << bcm_pin;
#line 80
    __nesc_atomic_end(__nesc_atomic); }
}

# 134 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
static bool SchedulerBasicP__Scheduler__runNextTask(void )
{
  uint8_t nextTask;

  /* atomic removed: atomic calls only */
#line 138
  {
    nextTask = SchedulerBasicP__popTask();
    if (nextTask == SchedulerBasicP__NO_TASK) 
      {
        {
          unsigned char __nesc_temp = 
#line 142
          FALSE;

#line 142
          return __nesc_temp;
        }
      }
  }
#line 145
  SchedulerBasicP__TaskBasic__runTask(nextTask);
  return TRUE;
}

#line 175
static void SchedulerBasicP__TaskBasic__default__runTask(uint8_t id)
{
}

# 75 "/home/wwhuang/git/tinyos-lab11/tos/interfaces/TaskBasic.nc"
static void SchedulerBasicP__TaskBasic__runTask(uint8_t arg_0x2ab95b77e9e0){
#line 75
  switch (arg_0x2ab95b77e9e0) {
#line 75
    case CC2520RpiReceiveP__receive_task:
#line 75
      CC2520RpiReceiveP__receive_task__runTask();
#line 75
      break;
#line 75
    case CC2520RpiSendP__sendDone_task:
#line 75
      CC2520RpiSendP__sendDone_task__runTask();
#line 75
      break;
#line 75
    case /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send:
#line 75
      /*CC2520RpiAmRadioC.PacketLinkLayerC.PacketLinkLayerP*/PacketLinkLayerP__0__send__runTask();
#line 75
      break;
#line 75
    case /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired:
#line 75
      /*HilTimerMilliC.AlarmToTimerC*/AlarmToTimerC__0__fired__runTask();
#line 75
      break;
#line 75
    case /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer:
#line 75
      /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__runTask();
#line 75
      break;
#line 75
    default:
#line 75
      SchedulerBasicP__TaskBasic__default__runTask(arg_0x2ab95b77e9e0);
#line 75
      break;
#line 75
    }
#line 75
}
#line 75
# 126 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
static uint32_t /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__getNow(void )
#line 126
{
  struct timespec timer_val;
  int t_ret;
  uint32_t time_now = 0;

  t_ret = clock_gettime(1, &timer_val);

  if (t_ret == 0) {

      time_now = timer_val.tv_sec * 1000 + timer_val.tv_nsec / 1000000;
    }

  return time_now;
}

# 70 "/home/wwhuang/git/tinyos-lab11/tos/lib/timer/VirtualizeTimerC.nc"
static void /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__fireTimers(uint32_t now)
#line 70
{
  uint16_t num;

  for (num = 0; num < /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__NUM_TIMERS; num++) {
      /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer_t *timer = &/*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__m_timers[num];

      if (timer->isrunning) {
          uint32_t elapsed = now - timer->t0;

          if (elapsed >= timer->dt) {
              if (timer->isoneshot) {
                timer->isrunning = FALSE;
                }
              else {
#line 83
                timer->t0 += timer->dt;
                }
              /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__Timer__fired(num);
              break;
            }
        }
    }
  /*HilTimerMilliC.VirtualizeTimerC*/VirtualizeTimerC__0__updateFromTimer__postTask();
}

# 170 "/home/wwhuang/git/tinyos-lab11/tos/system/SchedulerBasicP.nc"
static error_t SchedulerBasicP__TaskBasic__postTask(uint8_t id)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 172
    {
#line 172
      {
        unsigned char __nesc_temp = 
#line 172
        SchedulerBasicP__pushTask(id) ? SUCCESS : EBUSY;

        {
#line 172
          __nesc_atomic_end(__nesc_atomic); 
#line 172
          return __nesc_temp;
        }
      }
    }
#line 175
    __nesc_atomic_end(__nesc_atomic); }
}

# 65 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/timer/AlarmMilli32P.nc"
static void /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__Alarm__startAt(uint32_t t0, uint32_t dt)
#line 65
{
  struct timespec timer_val;
  int t_ret;
  uint32_t time_now;
  uint32_t elapsed;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
#line 71
    {
      t_ret = clock_gettime(1, &timer_val);

      if (t_ret == 0) {


          /*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__last_alarm = t0 + dt;


          time_now = timer_val.tv_sec * 1000 + timer_val.tv_nsec / 1000000;

          elapsed = time_now - t0;

          if (elapsed >= dt) {



              struct itimerspec new_timer = { { 0, 0 }, { 0, 0 } };

              new_timer.it_value.tv_sec = timer_val.tv_sec;
              new_timer.it_value.tv_nsec = timer_val.tv_nsec + 500000;

              t_ret = timer_settime(/*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__timerid, 1, &new_timer, (void *)0);
            }
          else {

              uint32_t remaining;
              struct itimerspec new_timer = { { 0, 0 }, { 0, 0 } };
              uint32_t remaining_seconds;
              uint32_t remaining_nanoseconds;

              remaining = dt - elapsed;
              remaining_seconds = remaining / 1000;
              remaining_nanoseconds = (remaining - remaining_seconds * 1000) * 1000000;

              new_timer.it_value.tv_sec = timer_val.tv_sec + remaining_seconds;
              new_timer.it_value.tv_nsec = timer_val.tv_nsec + remaining_nanoseconds;

              if (new_timer.it_value.tv_nsec > 999999999) {

                  new_timer.it_value.tv_nsec -= 999999999;
                  new_timer.it_value.tv_sec++;
                }

              t_ret = timer_settime(/*HilTimerMilliC.AlarmMilli32P*/AlarmMilli32P__0__timerid, 1, &new_timer, (void *)0);
            }
          if (t_ret != 0) {
            }
        }
    }
#line 120
    __nesc_atomic_end(__nesc_atomic); }
}

# 154 "/home/wwhuang/git/tinyos-lab11/tos/lib/rfxlink/layers/TinyosNetworkLayerC.nc"
static uint8_t /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosPacket__headerLength(message_t *msg)
{
  return /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubPacket__headerLength(msg) + /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__PAYLOAD_OFFSET;
}

#line 222
static message_t */*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__SubReceive__receive(message_t *msg)
{
  if (__nesc_ntoh_leuint8(/*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__getHeader(msg)->network.nxdata) == 0x3f) {
    return /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__TinyosReceive__receive(msg);
    }
  else {
#line 227
    return /*CC2520RpiAmRadioC.TinyosNetworkLayerC*/TinyosNetworkLayerC__0__Ieee154Receive__receive(msg);
    }
}

# 28 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/system/IOManagerP.nc"
static error_t IOManagerP__IO__registerFileDescriptor(uint8_t id, int file_descriptor)
#line 28
{
  ;

  if (!IOManagerP__is_init) {
      memset(IOManagerP__map, 0x01, sizeof(uint8_t ) * IOManagerP__N_FDS);
      IOManagerP__is_init = TRUE;
    }
  /* atomic removed: atomic calls only */
#line 35
  IOManagerP__map[id] = file_descriptor;
  return SUCCESS;
}

# 108 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
static void HplBcm2835GeneralIOP__Gpio__makeInput(uint8_t bcm_pin)
#line 108
{
  *(HplBcm2835GeneralIOP__gpio + bcm_pin / 10) &= ~(7 << bcm_pin % 10 * 3);
  /* atomic removed: atomic calls only */
#line 110
  HplBcm2835GeneralIOP__pin_output &= ~(1 << bcm_pin);
}

# 11 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/linux/BusyWaitMicroP.nc"
static void BusyWaitMicroP__BusyWait__wait(uint16_t dt)
#line 11
{
  struct timeval now;
#line 12
  struct timeval pulse;
  int micros;

  gettimeofday(&pulse, (void *)0);
  micros = 0;

  while (micros < (int )dt) {
      gettimeofday(&now, (void *)0);
      if (now.tv_sec > pulse.tv_sec) {
          micros = 1000000L;
        }
      else 
#line 22
        {
          micros = 0;
        }
      micros = micros + (now.tv_usec - pulse.tv_usec);
    }
}

# 104 "/home/wwhuang/git/raspberrypi-cc2520/tinyos/tos/chips/bcm2835/gpio/HplBcm2835GeneralIOP.nc"
static bool HplBcm2835GeneralIOP__Gpio__get(uint8_t bcm_pin)
#line 104
{
  return (((*(HplBcm2835GeneralIOP__gpio + 13) & (1 << bcm_pin)) >> bcm_pin) & 0x1) == 1;
}

