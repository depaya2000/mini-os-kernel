#include "../h/Scheduler.hpp"

buffer<_thread> Scheduler::threads;

_thread* Scheduler::get() {
    return threads.get();
}

void Scheduler::put(_thread *thread) {
    threads.put(thread);
}


/*	

sem_t joiner;
thread_t children[100];
int children_size = 0;

void _thread::thread_join(thread_t id){
	if(id -> get_state() == FINISHED) return;
	sem_wait(id->joiner);
}

void wait_for_children(){
	for(int i = 0; i < running-> childrenSize; i++){
		thread_join(running -> children[i]);
	}
}

int _thread::thead_exit() {
    running->state = FINISHED;
	sem_close(running->joiner);
    thread_dispatch();
    return -1;
}

_thread::_thread(run_function run, void *arg, char *stack_space) {
	sem_open(&this->joiner, 0);
}

int _thread::thread_create(_thread **handle, run_function run, void *arg, char *stack_space) {
    *handle = new _thread(run, arg, stack_space);
    if (*handle == nullptr) return -1;
    if (!running) {
        running = *handle;
    } else {
		running -> children[running -> childrenSize++] = *handle;
        Scheduler::put(*handle);
    }
    return 0;
}

class ThreadA: public Thread{
	public:
		Thread (int id): Thread() {this->id = id};
		void run() override{
			for(int i =0; i <10; i++){
				printInt(id);
				printString("/n");
				for(int j = 0; j < 100000000; j++);
				dispatch();
			}
		}
	private:
		int id;

};

void test8(){
	Thread* threads[10];
	for(int i = 0; i<10; i++){
		threads[i] = new ThreadA(i);
		threads[i]->start();
	}
	
	//for(int i = 0; i<10; i++){
	//	threads[i].join();
	//}
	
	wait_for_children();
	
	printString("END");
}


#define THREAD_GET_ID 0x15
#define THREAD_SET_MAX 0x16

int thread_get_id();                // vraća id i radi dispatch
int thread_set_max(int maxThreads); // postavlja maksimalan broj niti


int thread_get_id() {
    return (int) put_args(THREAD_GET_ID);
}

int thread_set_max(int maxThreads) {
    return (int) put_args(THREAD_SET_MAX, (uint64) maxThreads);
}


class Thread {
public:
    ...
    static void SetMaximumThreads(int num_of_threads); // user API
    ...
};


void Thread::SetMaximumThreads(int num_of_threads) {
    // poziva kernel syscall koji postavlja limit
    thread_set_max(num_of_threads);
}


int Thread::getMyId() { // opcionalno
    return thread_get_id();
}


case THREAD_GET_ID: {
    // postavimo povratnu vrednost u a0 (vrati ID trenutno running niti),
    // pa zatim izvršimo dispatch (promenu konteksta).
    uint64 ret = (uint64) _thread::get_running()->get_id();
    // postavi povratnu vrednost u a0 — vratit će se korisniku po povratku iz trap-a
    __asm__ volatile("mv a0, %0" :: "r"(ret));
    _thread::thread_dispatch(); // izvrši promenу konteksta
    break;
}
case THREAD_SET_MAX: {
    _thread::set_max_threads((int) arg1);
    break;
}


class _thread {
public:
    ...
    // id i pristupne metode
    int get_id() { return id; }

    // thread_join već implementiraš/imaš
    static int thread_join(_thread* handle);

    // novi: set/get max threads
    static void set_max_threads(int maxThreads);

private:
    ...
    int id;                      // jedinstveni id niti
    static int next_id;          // counter za dodelu idova

    // admission control (blokirane kreacije)
    static int max_threads;      // default 5
    static int active_count;     // broj trenutno "admitovanih" (aktivnih) niti
    static buffer<_thread> admission_queue; // red za nove, blokirane kreacije
};


_thread* _thread::running = nullptr;

// ID i admission kontrola
int _thread::next_id = 1; // ili 0 ako preferiraš
int _thread::max_threads = 5; // podrazumevano 5
int _thread::active_count = 0;
buffer<_thread> _thread::admission_queue;


_thread::_thread(run_function run, void *arg, char *stack_space) {
    this->run = run;
    this->arg = arg;
    this->stack_space = stack_space;
    this->context.ra = (uint64) &_thread::thread_start;
    this->context.sp = (uint64) &stack_space[DEFAULT_STACK_SIZE - 1];
    this->state = RUNNABLE;
    this->sem_state = SIGNALLED;

    this->id = next_id++;
}


int _thread::thread_create(_thread **handle, run_function run, void *arg, char *stack_space) {
    *handle = new _thread(run, arg, stack_space);
    if (*handle == nullptr) return -1;

    // Ako još nije inicijalizovana running nit (bootstrap), postavi je i povećaj broj aktivnih
    if (!running) {
        running = *handle;
        active_count++;
    } else {
        // Ako imamo mesta (manje od max), dodaj u scheduler
        if (active_count < max_threads) {
            Scheduler::put(*handle);
            active_count++;
        } else {
            // dosegnut limit -> nova nit se blokira u admission queue
            (*handle)->set_state(BLOCKED);
            admission_queue.put(*handle);
            // NE povećavamo active_count sve dok nit ne bude admitovana
        }
    }
    return 0;
}

int _thread::thead_exit() {
    // potencijalne prethodne buđenja (join_waiters) ili sem_close već radiš...
    // ... (ovde ubaci postojeću logiku za join_waiters / sem_close)

    // Obeleži nit kao FINISHED
    running->state = FINISHED;

    // Kada nit završava, smanji broj aktivnih niti (jer je jedna završila)
    if (active_count > 0) active_count--;

    // Ako postoje čekane kreacije, dozvoli jednu iz reda (FIFO) da postane runnable
    _thread* nextCreate = admission_queue.get();
    if (nextCreate) {
        // admituj sledeću nit
        nextCreate->set_state(RUNNABLE);
        Scheduler::put(nextCreate);
        active_count++; // sad je admitovana i ulazi u aktivni broj
    }

    thread_dispatch();
    return -1;
}

void _thread::set_max_threads(int maxThreads) {
    if (maxThreads <= 0) return;
    max_threads = maxThreads;
}




*/