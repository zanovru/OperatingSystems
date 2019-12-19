1. Установить обработчик atexit(), чтобы при завершении процесса выводилось на экран сообщение что сработал обработчик atexit(). Из текущего процесса создать дочерний. Каждый процесс должен вывести на экран признак дочерний/родительский, pid, ppid. Родительский  процесс должен дождаться завершения дочернего.

2. Реализовать аналог команды ls –la для текущего каталога. 

3a. Создать pipe. Создать дочерний процесс. Передать строчку через pipe из родительского в дочерний процесс с текущим временем и pid передающего процесса. Дочерний процесс должен вывети на экран его текущее время и полученную через pipe строчку.

3б. Аналогично 3а, но использовать fifo

4. Создать две независимых программы. В бесконечном цикле передавать через разделяемую память текущее время и pid передающего процесса. Принимающий процесс должен выводить на экран свое текущее время и pid и принятую строчку

5. Аналогично 4, но для синхронизации обмена данными использовать semaphore

6. Создать 10 читающих потоков и 1 пишущий. Потока должен быть доступен один и тот же массив символов. Пишущий поток записывает в общий массив номер записи(монотонно возрастающий счетчик), а читающиее потоки выводят на экран свой tid и текущее состояние общего массива. Доступ должен быть синхронизирован с помощью mutex

7. аналогично 6, но доступ должен быть синхронизирован с помощью блокировок чтения-записи

8. аналогично 6, но доступ должен быть синхронизирован с помощью условных переменных
