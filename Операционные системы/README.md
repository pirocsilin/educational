ОПЕРАЦИОННЫЕ СИСТЕМЫ.<br>
По курсу выполнены 3 лабораторные и 1 курсовой проект:

- Лабораторная №1.
Написать программу, которая должна “озвучивать” клавиатуру, т.е. после запуска этой программы нажатие любой клавиши на клавиатуре будет сопровождаться звуковым сигналом. Клавиатура при этом должна оставаться работоспособной, т.е. продолжать выполнять свои основные функции в нормальном темпе. Программа должна быть резидентной, т.е. оставаться в памяти после своего завершения.

- Лабораторная №2.
Написать резидентную программу, которая будет работать, как “будильник” – через заданный интервал времени издавать короткий звуковой сигнал – “тикать” (например, через 1–2 секунды). По завершении более длительного интервала времени (от нескольких секунд до нескольких минут или часов) должен раздаваться более продолжительный мелодичный звуковой сигнал, имитирующий звонок будильника.

- Лабораторная №3.
Написать программу, которая будет эмулировать параллельную работу некоторых потоков. Потоки должны работать циклически. В качестве модели использовать схему “производитель – потребитель”. Один поток (производитель) может помещать случайные (или какие-то определенные – например, только четные числа или квадраты целых чисел и т.п.) числа в буфер (массив заданного размера), для наглядности поток-производитель должен эти числа выводить на экран. Другой поток (потребитель) забирает числа из этого буфера.

- КУРСОВАЯ РАБОТА.
Необходимо написать программу, реализующую параллельную работу нескольких процессов. Каждый процесс может состоять из одного или нескольких потоков. Любой из потоков, работающих в составе этих процессов, может быть приостановлен и вновь запущен некоторой определенной клавишей (можно использовать буквенные или цифровые клавиши). Нажатия клавиш обрабатывать с помощью прерывания от клавиатуры:
Процесс 1. Два потока: первый читает информацию из файла  в буфер, второй эту информацию из буфера выдаёт на экран. При заполнении окна вывода до конца его содержимое не должно обновляться полностью – вывод новой информации должен осуществляться в последнюю строку, а все остальные строки смещаться вверх. Имя читаемого файла задавать как параметр командной строки. После окончания файла он начинает считываться заново.
Процесс 2. Два потока: один выполняет поиск всех последовательных простых чисел и заносит их в буфер, второй осуществляет их вывод из буфера на экран.
Процесс 3. Музыкальное сопровождение – как минимум несколько нот различной длительности, образующие мелодию. Во включённом состоянии мелодия должна играть в фоновом режиме.
Процесс 4. Вывод заголовка работы (фамилия и имя автора и название работы) в виде «бегущей строки». Место вывода (верхняя или нижняя строка экрана) задавать параметром командной строки.
