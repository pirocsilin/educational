uses Crt, Dos;

type
	DataArr		= Array [0..0] of integer;
	PtrDataArr	= ^DataArr;
const 
	Impuls = 18.2;
	n = 10;
var
	OldPOP				:	procedure;
	Buffer				: 	array [1..n] of Byte;
	StreamsArr			:	PtrDataArr;
	PriorityActive, ManyStreams 	:	boolean;
	Kvant, CountElem, Tail, Head	:	Byte;
	Cod1, Cod2, DelayMS		:	integer;
	x,y,i, CountP, CountC		:	integer;
	PriorityMode, Timer, EndTime	:	integer;
	CallProc, Streams 		:	integer;
	StartTime			:	Real;
	ExitCode			:	Byte;
	StatusProd, StatusCons		:	boolean;

procedure Produce;  			forward;
procedure Consumer; 			forward;
procedure Put(x,y,i,col  : integer);	forward;
procedure BufferFull; 			forward;

procedure Produce;			{Поток-производитель}
begin
	while StatusProd = true do
	begin
		if (CountElem = n) and (Streams>2) then
			begin
				StatusCons := true;	{Если буфер полный и имеем очередь потоков,	}
				StatusProd := false;	{то активируем поток-потребитель и завершаем    }
				exit;			{работу производителя. Тогда в очереди будет    }
			end				{найден ближайший потребитель который получит   }
							{оставшийся квант времени.			}
		else if CountElem = n then
			begin				{Если буферное пространство заполнено, то	}
				StatusCons := true;	{меняем статус потока потребителя на активный	}
				Consumer; 		{и передаем управление. Таким образов оставшийся}
			end				{квант времени отдаем потребителю.		}
		
		else begin
			inc(CountElem);
			BufferFull;
			Buffer[Tail] := random(10);

			gotoxy(x,y+3); clreol; write('Produce:  ', Buffer[Tail]);
			gotoxy(x,y+4); clreol; write('Consumer: NO ACTIVE');
			Put(x,y,Tail-1,1);
			
			Tail := Tail mod n + 1;
			
			delay(DelayMS);
		end;
	end;
end;

procedure Consumer;			{Поток-потребитель}
begin	
	while StatusCons = true do
	begin
		if (CountElem = 0) and (Streams>2) then 
			begin
				StatusProd := true;	{Если буфер пуст и имеем очередь потоков,	}
				StatusCons := false;	{то активируем поток-производитель и завершаем  }
				exit;			{работу потребителя. Тогда в очереди будет  	}
			end				{найден ближайший производитель который получит	}
							{оставшийся квант времени.			}
		
		else if (ExitCode<>0) and (CountElem = 0) then exit

		else if CountElem = 0 then
			begin				{Если буферное пространство пустое, то		}
				StatusProd := true;	{меняем статус потока производителя на активный	}
				Produce; 		{и передаем управление. Таким образов оставшийся}
			end				{квант времени отдаем производителю.		}

		else begin
			dec(CountElem);
			BufferFull;
			
			gotoxy(x,y+4); clreol; write('Consumer: ', Buffer[Head]);
			gotoxy(x,y+3); clreol; write('Produce:  NO ACTIVE');
			Put(x,y,Head-1,9);
			
			Head := Head mod n + 1;
			
			delay(DelayMS);
		end;
	end;
end;

procedure Header;			  {Вывод заголовка и вспомогательного контента}
begin
	textcolor(13);
	gotoxy(x-9, y-11);
	write('Format: Lab_3 [1] [2] {[1] - Delay(MS),');
	gotoxy(x-1, y-10);
	write('[2] - on (Priority Mode) OR Integer>3 (Streams)}');
	textcolor(10);
	gotoxy(x, y-4);
	if PriorityMode=1 then write('PRIORITY MODE');
	gotoxy(x, y-2);
	textcolor(12);
	write('Buffer:');
	gotoxy(x + n*3 + 4, y-2);
	write('Full:');
	textcolor(14);
	gotoxy(x-9, y+7);
	write('Press ESC to stop');
	textcolor(15);
end;

procedure BufferFull;		  {Процедура вывода заполненности буфера}
begin
	textcolor(10);
	gotoxy(x+14, y-4);
	if PriorityActive then write('-> actyive') else clreol;
	gotoxy(x + n*3 + 4, y); 
	clreol;
	textbackground(10);
	textcolor(4);
	write(CountElem / n * 100:4:0, ' % ');
	textbackground(0);
	textcolor(15);
	gotoxy(33, y+7);
end;

procedure Put;			{Процедура вывода содержания буфера}
begin
	textbackground(col);
	gotoxy(x+i*3,y);
	if col = 9 then 
		write(' _ ') 
	else 
		write(' ',Buffer[i+1],' ');
	textbackground(0);
end;

procedure CallStreams;		  {Процедура управления несколькими потоками}
var
	Index : Integer;
	Count : Integer;
	NumP, NumC : Integer;
begin
	BufferFull;
	Index := 0;			{Индекс текущего потока в списке  }
	NumP  := 0;			{Количество потоков-производителей}
	NumC  := 0;			{Количество поток-потребитель	}
	Count := CountP + CountC; 	{Общее количество потоков	}
	
	textcolor(10);
	gotoxy(x, y-5);
	write('Produce ',CountP,' | Consumer ',CountC);
	textcolor(15);
	
	if (CountP=0) or (CountC=0) then 		{Проверка числа потребителей и 		}
	begin						{производителей. Если все потребители	}
		textcolor(14);				{или производители, то завершаем программу }
		gotoxy(x-9, y+7);			{с выводом сообщения об ошибке.	}
		write('[ERROR]: Produce=0 OR Consumer=0');
		halt;
	end;

	repeat
		if (StreamsArr^[Index]=0) and (StatusProd=true) then
			begin
				gotoxy(x-5,y+3);				{Если очередной поток - производитель	}
				write('    ');					{и если позволяет статус активности, 	}
				gotoxy(x-5,y+3);				{то вызываем его на выполнение.		}
				write('[',NumP + 1,']');			{Если очередь подошла к производителю, 	}
				inc(NumP);					{а его статус - не активен, т.е. активен}
				NumP := NumP mod CountP;			{потребитль, то переходим к следующему потоку}
				Produce;					{пока не найдем очередного потребителя.	}
			end
		else if (StreamsArr^[Index]=1) and (StatusCons=true) then
			begin
				gotoxy(x-5,y+4);				{Аналогично потоку производителю	}
				write('    ');
				gotoxy(x-5,y+4);
				write('[',NumC + 1,']');
				inc(NumC);
				NumC := NumC mod CountC;
				Consumer;
			end;
				
		inc(Index);
		Index := Index mod Count;
	until ExitCode = 1;
end;

{$F+}
procedure NewPOP; interrupt;
var
	ch : Char;
begin
	inc(Timer);
	
	if Timer > EndTime then 			{Если время работы очередного потока исчерпано}
	begin
		if Timer > 1000 then Timer := 1;
		
		StatusProd := false;			{Изменяем статусы потоков на не активные}
		StatusCons := false;
		
		CallProc:= Random(2);			
		Kvant 	:= Random(5) + 2;
		EndTime	:= Timer + Kvant;
		
		if(CallProc = 0) then 			{Изменяем статус очередного активного потока}
			StatusProd := true
		else 
			StatusCons := true;
	end;
	
	if keypressed then 
	begin
		ch := readkey;
		if ord(ch) = 27 then 
			begin
				ExitCode := 1;				{Если нажали клавишу ESC то  	}
				StatusProd := false;			{останавливаем потоки, меняя	}
				StatusCons := false;			{их статус на не активные и 	}
				SetIntVec($8, Addr(OldPOP));		{возвращаем старую процедуру 	}
			end;						{обработки прерывания таймера	}
	end;									
	
	inline($9C);
	OldPOP;	
end;
{$F-}

procedure PriorityCall;					{Проверка состояния буфера	}
begin
	if CountElem / n * 100 >= 90 then begin		{Если заполненность буфера от 90%,  }
		PriorityActive := true;	 		{то вызываем поток-потребитель.	    }
		StatusCons := true;
		Consumer; end
	else if CountElem / n * 100 <= 10 then begin	{Если буфер заполнен на 10% и менее,}
		PriorityActive := true;			{то вызываем поток-производитель.} 
		StatusProd := true;
		Produce; end;
	PriorityActive:= false;
end;

begin
	Timer	  := 0;
	EndTime	  := 0;
	CountP	  := 0;			{Количество потоков-производителей }
	CountC	  := 0;			{Количество потоков-потребителей   }
	CountElem := 0;			{Количество элементов в буфере	   }
	Tail 	  := 1;			{Указатель на "Хвост" буфера	   }
	Head 	  := 1;			{Указатель на "Голову" буфера	   }
	ExitCode  := 0;			{Состояние нажатия клавиши	   }
	y:=13;   x:= 25;		{Базовые координаты - Начало буфера}
	PriorityActive := false;

	val(ParamStr(1), DelayMS, Cod1);		{Получаем пераметр 1}
	val(ParamStr(2), Streams, Cod2);		{Получаем пераметр 2}	
	
	if ParamStr(2) = 'on' then			{Проверка приоритетного режима}
		PriorityMode := 1
	else 
		PriorityMode := 0;
		
	randomize;	
	if (Cod2=0) and (Streams>2) then		{Проверка многопоточного режима,}
		begin					{если активен то создаем список }
			ManyStreams := true;		{потоков с количеством Streams.	}
			for i:=0 to Streams-1 do begin 
			CallProc := random(2);
			StreamsArr^[i] := CallProc;
			if CallProc=0 then inc(CountP) else inc(CountC); end;
		end
	else
		ManyStreams := false;
	
	textmode(3);
	clrscr;

	if (Cod1<>0) or (DelayMS<10) then		{Проверка первого параметра - Задержка}
	begin
		textcolor(12);
		gotoxy(x-9, y-8);
		write('WARNING: ');
		textcolor(15);
		gotoxy(x, y-8);
		write('Delay must be Integer and not less 10 ms');
		gotoxy(x, y-7);
		write('Program started with a delay of 10 ms!');
		DelayMS := 10;
	end;
	
	Header;
	for i:=0 to n-1 do Put(x,y,i,9);

	GetIntVec($8, @OldPOP);
	SetIntVec($8, Addr(NewPOP));
	
	if ManyStreams then 							{Если режим многопоточности активен, то	}
		CallStreams							{управление вызовами потоков передаем в	}
	else 
		begin								{функцию CallStreams. Иначе работаем в 	}
			repeat							{режиме двух потоков.			}
				if PriorityMode = 1 then PriorityCall; 		{Если активен приоритетный режим	}
										{то проверяем состояние буфера. 	}
				if StatusProd = true then
					Produce								
				else if StatusCons = true then 
					Consumer;
						
			until ExitCode = 1;					{Бесконечный цикл пока не нажали ESC}
		end;
		
	StatusCons := true;							{Дочитываем информацию из буфера	} 
	Consumer;
	gotoxy(x+10,y+4);
	textcolor(12);
	write('BUFFER EMPTY');
	gotoxy(x-9, y+7);
	writeln;
end.
