#include <iostream>

using namespace std;


#define LL_INSERT(item,list) do{ \
      item->prev = NULL;         \
      item->next = list;         \
      if(list != NULL) list->prev = item; \
      list = item;                           \
}while(0)


#define LL_REMOVE(item,list) do{ \
       if(item->prev != NULL) item->prev->next = item->next; \
       if(item->next != NULL) item->next->prev = item.prev;  \
       if(list == item) list = item.next; \
       item.prev = item.next = NULL; \
}while(0)


struct NWORKER{
    //线程id
    pthread_t  id;
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
    struct NWORKER *workers;
    struct NTASK *tasks;

    //互斥锁
    pthread_mutex_t mtx;
    //条件变量
    pthread_cond_t  cond;
} threadpool_t;

// static -> entry's callback

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

    }

    return 0;
}

// destroy thread pool
int thread_pool_destroy(struct NMANAGER* pool){

    return 0;
}

// push thread pool








int main() {
    std::cout << "Hello, World!" << std::endl;


    return 0;
}
