#include <iostream>

using namespace std;


#define LL_INSERT(item,list) do{ \
      item->prev = NULL;         \
      item->next = list;         \
      if(list != NULL) list->prev = item; \
      list = item;                           \
}while(0);


#define LL_REMOVE(item,list) do{ \
       if(item->prev != NULL) item->prev->next = item->next; \
       if(item->next != NULL) item->next->prev = item->prev;  \
       if(list == item) list = item->next; \
       item->prev = item->next = NULL; \
}while(0);


struct NWORKER{
    //线程id
    pthread_t  id;

    int terminate;

    struct NMANAGER *pool;
    struct NWORKER *prev;
    struct NWORKER *next;
};

struct NTASK {
    void (*task_func)(void *arg);
    void *user_data;

    struct NTASK *prev;
    struct NTASK *next;
};

typedef struct NMANAGER{
    //执行队列
    struct NWORKER *workers;
    //任务
    struct NTASK *tasks;

    //互斥锁
    pthread_mutex_t mtx;
    //条件变量
    pthread_cond_t  cond;
} threadpool_t;

// static -> entry's callback
static void* thread_callback(void *arg){
    struct NWORKER *work = (struct NWORKER *)arg;
    while (1){
        //互斥锁 lock和unlock是配套使用的。 如下是固定写法
        pthread_mutex_lock(&work->pool->mtx);
        //当线程池，任务为空，就进入条件等待
        while (work->pool->tasks == NULL){
            //条件等待两个方法：pthread_cond_wait和pthread_cond_signal
            pthread_cond_wait(&work->pool->cond,&work->pool->mtx);

            //terminate == 1 代表线程已退出
            if(work->terminate) break;
        }

        //terminate == 1 代表线程已退出
        if(work->terminate){
            //退出循环前，需要解锁
            pthread_mutex_unlock(&work->pool->mtx);
            break;
        }

        struct NTASK *task = work->pool->tasks;
        //将任务，从任务队列中移除
        if(task){
            LL_REMOVE(task,work->pool->tasks);
        }
        pthread_mutex_unlock(&work->pool->mtx);

        //取出任务，执行
        task->task_func(task);
    }
}

// create thread pool
int thread_pool_create(struct NMANAGER* pool,int nthread){
    if(pool == NULL) return -1;
    if(nthread < 1) nthread = 1;
    //创建时，记得将线程池清零
    memset(pool,0,sizeof (struct NWORKER));

    // 创建一个互斥锁
    pthread_mutex_t blank_mtx = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->mtx,&blank_mtx, sizeof(blank_mtx));

    // 创建一个条件变量
    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->cond,&blank_cond, sizeof(blank_cond));

    int idx = 0;
    for (int idex = 0; idex < nthread; ++idex) {
        struct NWORKER *worker = (struct NWORKER *)(malloc(sizeof(struct NWORKER)));
        //当分配的内存为空，直接退出
        if(worker == NULL){
            perror("malloc");
            exit(1);
        }

        /**
         * memset 是 C++ 中的一个内存操作函数，用于将给定值填充到给定内存区域。
         * void *memset(void *ptr, int value, size_t n);
         * ptr:指向要填充的内存区域的指针， value: 填充的值， n:要填充的字节数。
         * memset 函数的返回值是指向  ptr  所指向的内存区域的指针。
         */
        memset(worker,0,sizeof(struct NWORKER));

        //将线程池，指向到执行队列，这样在回调函数里面，就可以直接获取到线程池信息。
        worker->pool = pool;

        /**
         *  thread ：指向新线程的标识符的指针。
         *  attr ：指向线程属性结构的指针。如果为 NULL，则使用默认属性。
         *  start_routine ：线程执行的函数。
         *  arg ：传递给  start_routine  的参数。
         */
        int ret = pthread_create(reinterpret_cast<pthread_t *>(worker->id), NULL, thread_callback, worker);

        //非0，代表创建失败，清空worker，退出
        if(ret){
            perror("pthread_create");
            free(worker);
            exit(1);
        }

        LL_INSERT(worker,pool->workers);
    }
    return idx;
}


// destroy thread pool
int thread_pool_destroy(struct NMANAGER* pool){
    struct NWORKER *worker = NULL;
    for(worker == pool->workers; worker != NULL;worker->next){
        //线程退出，将terminate设为1
        worker->terminate = 1;
    }
    //加锁
    pthread_mutex_lock(&pool->mtx);
    //线程同步函数，用于唤醒所有等待在指定条件变量上的线程。
    pthread_cond_broadcast(&pool->cond);
    //解锁
    pthread_mutex_unlock(&pool->mtx);
    return 0;
}

// push thread pool
int thread_pool_push(struct NMANAGER* pool,struct NTASK *task){
    //加锁
    pthread_mutex_lock(&pool->mtx);
    //添加任务
    LL_INSERT(task,pool->tasks);
    //通知任务，thread_callback里面会获取到条件变量
    pthread_cond_signal(&pool->cond);
    //解锁
    pthread_mutex_unlock(&pool->mtx);
    return 0;
}








int main() {
    std::cout << "Hello, World!" << std::endl;
    struct NMANAGER *pool;
    int nthread = 20;
    int ret = thread_pool_create(pool,nthread);


    return 0;
}
