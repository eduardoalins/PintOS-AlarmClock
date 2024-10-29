// thread.c

void thread_add_sleep(int64_t time){ // função para adicionar o elemento na lista sleep
  struct thread *cur = thread_current (); // vai pegar a thread atual e colocar no ponteiro cur
  enum intr_level old_level; // pegar o level

  ASSERT (!intr_context ());

  old_level = intr_disable (); // desabilitar interrupções de acordo com o level
  
  if (cur != idle_thread) {
    list_push_back (&sleep_list, &cur->elem); // adicionando o elemento na ultima posição da sleep_list
    cur -> status = THREAD_BLOCKED; // colocando a thread como blocked dado o enum de thread status
    cur -> wake_tick = time; //adiciona na thread em qual tick ele deve acordar
  }

  schedule (); // schedule para liberar a CPU
  intr_set_level (old_level); // retorna as interrupções
}

void thread_wakeup(int64_t tick){ // função para verificar se o elemento acordou e tira-lo da sleep list
  struct list_elem *n = list_begin(&sleep_list); // criando um elemento do tipo struct list para poder percorrer-la  

  while (n != list_end(&sleep_list)){
    struct thread *e = list_entry(n, struct thread, elem); // list_entry para "pegar" o elemento elem na posição n da lista sleep 
    if (e->wake_tick <= tick){ // verificando se o tick atual é maior ou igual ao tick que a thread deveria acordar
      struct list_elem *temp = n; // colocando um temporario para quando remover não perder a referencia da lista
      n = list_next(n); // passando para o proximo elemento antes de remover
      temp = list_remove(temp); // removendo da lista e colocando o elemento removido em temp mas sem propósito
      list_push_back(&ready_list, &e->elem); // adicionando o elemento atual "e" na ultima posição da lista de ready
      e->status = THREAD_READY; // mudando o status da thread
    }else{ // colocando o else para não acabar passando duas vezes o termo
      n = list_next(n); // apenas passando o elemento
    }
  }

}

static struct list sleep_list; // criação da lista de espera

void
thread_init (void) 
{
  ASSERT (intr_get_level () == INTR_OFF);

  lock_init (&tid_lock);
  list_init (&ready_list);
  list_init (&all_list);
  list_init (&sleep_list); // iniciando a sleep_list

  /* Set up a thread structure for the running thread. */
  initial_thread = running_thread ();
  init_thread (initial_thread, "main", PRI_DEFAULT);
  initial_thread->status = THREAD_RUNNING;
  initial_thread->tid = allocate_tid ();
}

// thread.h

void thread_add_sleep(int64_t time); //declarando a função add_sleep
void thread_wakeup(int64_t tick); // declarando a função thread_wakeup

struct thread
  {
    /* Owned by thread.c. */
    tid_t tid;                          /* Thread identifier. */
    enum thread_status status;          /* Thread state. */
    char name[16];                      /* Name (for debugging purposes). */
    uint8_t *stack;                     /* Saved stack pointer. */
    int priority;                       /* Priority. */
    struct list_elem allelem;           /* List element for all threads list. */

    /* Shared between thread.c and synch.c. */
    struct list_elem elem;              /* List element. */
    int64_t wake_tick;  // elemento para saber em qual tick a thread ira acordar

#ifdef USERPROG
    /* Owned by userprog/process.c. */
    uint32_t *pagedir;                  /* Page directory. */
#endif

    /* Owned by thread.c. */
    unsigned magic;                     /* Detects stack overflow. */
  };

// timer.c

void
timer_sleep (int64_t ticks) 
{
  int64_t start = timer_ticks (); // pegando a quantidade atual de ticks
  int64_t tempo_final = start + ticks; // somando com a quantidade de ticks que devemos esperrar para poder saber em qual tick ele irá acordar

  ASSERT (intr_get_level () == INTR_ON);
  
  if (timer_elapsed (start) < ticks) // tempo que passou de formal alguma pode ser maior ou igual a quantidade de ticks
  {
    thread_add_sleep(tempo_final); // adiciona na lista de espera (sleep_list)
  }
}

static void
timer_interrupt (struct intr_frame *args UNUSED)
{
  ticks++;
  thread_tick ();
  thread_wakeup(ticks); // chamando a função para ficar verificando se a thread acordou
}