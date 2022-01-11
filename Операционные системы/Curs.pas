
uses Crt, Dos;

const
	FIO : string = '<< Full Name "Course work" >>';
	Arr : array [0..47] of integer = (659, 622, 659, 622, 659, 494,
					 				  587, 523, 440, 440, 440, 262,
									  330, 440, 494, 494, 494, 330,
					 				  415, 494, 523, 523, 523, 330,
									  659, 622, 659, 622, 659, 494,
					 				  587, 523, 440, 440, 440, 262,
					  				  330, 440, 494, 494, 494, 330, 
					  				  523, 494, 440, 440, 440, 330);
	MaxSpeed = 5;
	n = 5;
var
	FileName			:	String;
	Buffer1				: 	array [1..n] of Char;
	Buffer2				: 	array [1..n] of longint;
	SimpleNum			:	longint;
	OldPOP1				:	procedure;
	OldPOP2				:	procedure;
	CountElem1, CountElem2, dP_3	:	Byte;
	Code, Head1, Teil1, dC_3	:	Byte;
	Timer, Stream, Head2, Teil2	:	Byte;
	dP_1, dC_1, dP_2, dC_2, DT	:	Byte;
	xP1, yP1, xP2, yP2, xP3, yP3	:	Byte;
	Position, p1, p2, ExitCode  	:	Byte;
	DT1, DT2			:	Integer;
	ch				:	Char;
	f				:	File of Char;
	CountCh, fSize, Note, C1, C2	:	Integer;
	TickerPos, PrevTimer		:	Byte;
	StatusProd1, StatusCons1	:	Boolean;
	StatusProd2, StatusCons2	:	Boolean;
	StopProd2, StopCons2		:	Boolean;
	AwaitProd2, AwaitCons2		:	Boolean;
	
procedure Consumer_1;			forward;
procedure Produce_1;			forward;
procedure Consumer_2;			forward;
procedure Produce_2;			forward;
procedure SpeedControl(Num : Byte); 	forward;
procedure GoXY(ProcNum : Byte);		forward;
procedure Put(x,y,col,p  : integer);	forward;
procedure Condition(x,y,n : Byte);	forward;
procedure BufferFull(x,y  : Byte);	forward;

procedure SpeedControl;						{Управление скоростью потоков}
begin
	case Num of
		16: begin 
				inc(dP_1);
				dP_1 := dP_1 mod MaxSpeed; 
				Put(20,19,11,dP_1);
			end;
		17: begin 
				inc(dC_1);
				dC_1 := dC_1 mod MaxSpeed; 
				Put(20,20,7,dC_1);
			end;
		18: begin 
				inc(dP_2);
				dP_2 := dP_2 mod MaxSpeed; 
				Put(62,19,11,dP_2);
			end;
		19: begin 
				inc(dC_2);
				dC_2 := dC_2 mod MaxSpeed; 
				Put(62,20,7,dC_2);
			end;
		20: begin 
				DT1 := DT1 - 3;
				if DT1 < 0 then DT1 := 9;
				inc(p1);
				p1 := p1 mod 4;
				Put(18,6,11,p1);
			end;
		21: begin 
				DT2 := DT2 - 3;
				if DT2 < 0 then DT2 := 9;
				inc(p2);
				p2 := p2 mod 4;
				Put(49,6,11,p2);
			end;
	end;
end;

procedure GoXY;							{Переход в нужную позицию окна}
begin
	case ProcNum of
		1: begin 
				window(5,8,59,17);
				gotoxy(xP1, yP1);
			end;
		2: begin 
				window(63,8,74,17);
				gotoxy(xP2, yP2);
			end;
		3: begin 
				window(1,1,80,25); 
				gotoxy(1, yP3); 
				clreol; 
				gotoxy(xP3, yP3);
			end;
		4: begin 
				window(1,1,80,25); 
				gotoxy(21,24);
				clreol;
			end;
	end;
end;
	
procedure Produce_1;						{Поток - производитель первого процесса}
var
	CheckDivisorZero : byte;
begin
	if dP_1=0 then begin Condition(26,19,3); exit; end;
	while StatusProd1 = true do
	begin
		if (CountElem1 = n) and (dC_1 = 0) then 
		begin 
			Condition(26,19,2); 
			BufferFull(36,20);
			exit; 
		end
		else if CountElem1 = n then
			begin							{Если буферное пространство заполнено, то	}
				StatusCons1 := true;		{меняем статус потока потребителя на активный	}
				Consumer_1; 				{и передаем управление. Таким образов оставшийся}
			end								{квант времени отдаем потребителю.		}
		else
		begin
			Condition(26,19,1);
			GoXY(1);
			read(f,ch);
			Buffer1[Teil1] := ch;

			inc(CountElem1);
			inc(CountCh);
			
			if CountCh = fSize then begin
				seek(f,0);
				CountCh := 0; end;
			
			Teil1 := Teil1 mod n + 1;

			CheckDivisorZero := dP_1;
			if CheckDivisorZero = 0 then CheckDivisorZero := 1;
			delay(round(55.0 / (CheckDivisorZero * 3)));
		end;
	end;
	BufferFull(36,20);
end;

procedure Consumer_1;						{Поток - потребитель первого процесса}
var
	CheckDivisorZero : byte;
begin
	if dC_1=0 then begin Condition(26,20,3); exit; end;
	while StatusCons1 = true do
	begin
		if (CountElem1 = 0) and (ExitCode = 2) then 
		begin
			BufferFull(36,20);
			StatusCons1 := false;
			StatusProd1 := false;
			exit; 
		end
		else if (CountElem1 = 0) and (dP_1 = 0)then 
		begin 
			Condition(26,20,2); 
			BufferFull(36,20);
			exit; 
		end
		else if CountElem1 = 0 then 
		begin
			StatusProd1 := true;
			Produce_1;
		end
		else
		begin
			Condition(26,20,1);
			GoXY(1);
			write(Buffer1[Head1]);
			
			dec(CountElem1);
			Head1 := Head1 mod n + 1;
			
			xP1 := whereX; 
			yP1 := whereY;

			CheckDivisorZero := dC_1;
			if CheckDivisorZero = 0 then CheckDivisorZero := 1;
			delay(round(55.0 / (CheckDivisorZero * 3)));
		end;
	end;
	BufferFull(36,20);
end;

procedure Produce_2;							{Поток - производитель второго процесса}
var
	CheckDivisorZero : byte;
	i,j  : longint;
	k	 : Integer;
	Label L1,L2;
begin
	if (dP_2=0) and (StopProd2=false) then 		{Если поток остановлен, обновляем статус}
	begin 
		Condition(68,19,3); 
		StopProd2 := true;
		exit; end
	else if dP_2=0 then exit;
	StopProd2 := false;
	
	while StatusProd2 = true do
	begin
		if (CountElem2 = n) and (dC_2 = 0) and (AwaitProd2=false) then 
		begin 
			Condition(68,19,2); 
			BufferFull(75,20);
			AwaitProd2 := true;
			exit; end
		else if (CountElem2 = n) and (dC_2 = 0) then exit;
		AwaitProd2 := false;

		if CountElem2 = n then
			begin						{Если буферное пространство заполнено, то	}
				StatusCons2 := true;	{меняем статус потока потребителя на активный	}
				Consumer_2; 			{и передаем управление. Таким образов оставшийся}
			end							{квант времени отдаем потребителю.		}
		else
		begin
			Condition(68,19,1);
			GoXY(2);
			L1:
			for i:=1 to MaxLongint do 
			begin
				inc(SimpleNum);
				if (SimpleNum=2) or (SimpleNum=3) then goto L2;
				if SimpleNum=MaxLongint then begin SimpleNum:=1; goto L1; end;
				if odd(SimpleNum) then
				begin
					for j:=2 to trunc(sqrt(SimpleNum)) do
					begin
						if SimpleNum mod j = 0 then goto L1;
					end;
					goto L2;
				end;
			end;
			L2:
			
			Buffer2[Teil2] := SimpleNum;
			inc(CountElem2);
			
			Teil2 := Teil2 mod n + 1;
			
			CheckDivisorZero := dP_2;
			if CheckDivisorZero = 0 then CheckDivisorZero := 1;
			delay(round(55.0 / (CheckDivisorZero * 3)));
		end;
	end;
	BufferFull(75,20);
end;

procedure Consumer_2;								{Поток - потребитель второго процесса}
var
	CheckDivisorZero : byte;
	i:Integer;
begin
	if (dC_2=0) and (StopCons2=false) then 			{Если поток остановлен, обновляем статус}
	begin 									
		Condition(68,20,3); 
		StopCons2 := true;
		exit; end
	else if dC_2=0 then exit;
	StopCons2 := false;

	while StatusCons2 = true do
	begin
		if (CountElem2 = 0) and (ExitCode = 2) then 
		begin
			BufferFull(75,20);
			StatusCons2 := false;
			StatusProd2 := false;
			exit; 
		end
		else if (CountElem2 = 0) and (dP_2 = 0) and (AwaitCons2=false) then 
		begin 
			Condition(68,20,2); 
			BufferFull(75,20);
			AwaitCons2 := true;
			exit; end
		else if (CountElem2 = 0) and (dP_2 = 0) then exit;
		AwaitCons2 := false;
		
		if CountElem2 = 0 then
		begin
			StatusProd2 := true;
			Produce_2;
		end
		else
		begin
			Condition(68,20,1);
			GoXY(2);
			if xP2 <> 0 then writeln;
			write(Buffer2[Head2]);
			
			xP2 := whereX; 
			yP2 := whereY;
			
			dec(CountElem2);
			Head2 := Head2 mod n + 1;
			
			CheckDivisorZero := dC_2;
			if CheckDivisorZero = 0 then CheckDivisorZero := 1;
			delay(round(55.0 / (CheckDivisorZero * 3)));
		end;
	end;
	BufferFull(75,20);
end;

procedure Ticker;							{Процесс третий - Бегущая сторока}
var 
	i:Integer;
begin
	if DT1 = 0 then begin Condition(23,6,3); exit; end
	
	else if Timer mod DT1 = 0 then
	begin
		Condition(23,6,1);
		GoXY(3);
		Position := xP3;
		for i:=1 to length(FIO) do
		begin
			write(FIO[i]);
			inc(xP3);
			if xP3 = 81 then xP3 := 1;
			gotoxy(xP3, yP3);
		end;
		dec(Position);
		if Position = 0 then Position := 80;
		xP3 := Position;
	end;
end;

procedure Music;							{Процесс четвертый - Музыка}
begin
	if DT2 = 0 then 
	begin 
		Condition(54,6,3); 
		nosound; 
		exit; 
	end
	
	else if Timer mod DT2 = 0 then
	begin
		Condition(54,6,1);
		nosound;
		sound(Arr[Note]);
		inc(Note);
		Note := Note mod 48;
	end;
end;

{$F+}
procedure NewPOP1; interrupt;
var
	ch : Char;
begin
	inc(Timer);
	if Timer = 19 then Timer := 1;
	
	case Timer of
		3,6,9,12,15,18: begin
					Music;
					Ticker;
					GoXY(2);
				end;
		1,4,7,10,13,16: begin
					StatusProd2 := false;
					StatusCons2 := false;
					Stream := random(2);
					if Stream = 0 then
						StatusProd1 := true
					else
						StatusCons1 := true;
					end;
		2,5,8,11,14,17: begin
					StatusProd1 := false;
					StatusCons1 := false;
					Stream := random(2);
					if Stream = 0 then
						StatusProd2 := true
					else
						StatusCons2 := true;
					end;
	end;
	
	if ExitCode = 1 then									{Выход, если нажали ESC}
	begin
		ExitCode := 2;
		SetIntVec($8, Addr(OldPOP1));
	end
	else if (dP_1=0) and (dC_1=0) and (dP_2=0) and			{Выход, если все	}
			(dC_2=0) and (p1=0)   and (p2=0)   then 		{процессы приостановлены}
	begin
		ExitCode := 2;
		SetIntVec($8, Addr(OldPOP1));					{Возврат адресов системных}
		SetIntVec($9, Addr(OldPOP2));					{прерываний в таблицу	  }
		inline($9C);
		OldPOP2;
		if keypressed then ch := readkey;
	end;

	inline($9C);
	OldPOP1;	
end;

procedure NewPOP2; interrupt;
var
	ch : char;
begin
	Code := port[$60];				{Выбор нажатой клавиши для   }
	case Code of					{управления скоростью потоков}
		16: SpeedControl(16);
		17: SpeedControl(17);
		18: SpeedControl(18);
		19: SpeedControl(19);
		20: SpeedControl(20);
		21: SpeedControl(21);
		1: begin					
				ExitCode := 1;
				SetIntVec($9, Addr(OldPOP2));
			end;
	end;
	inline($9C);
	OldPOP2;

	if keypressed then ch := readkey;	
end;
{$F-}
	
procedure Ramka(x,y : byte);		{Стиль оформления приложения}
var
	i:Integer;
begin
	clrscr;
	gotoxy(x,y); writeln(chr(200));
	gotoxy(x+56,y); writeln(chr(202));
	gotoxy(x+72,y); writeln(chr(188));
	for i:=1 to 10 do
	begin
		gotoxy(x,y-i); writeln(chr(186));
		gotoxy(x+56,y-i); writeln(chr(186));
		gotoxy(x+72,y-i); writeln(chr(186));
	end;
	gotoxy(x,y-11); writeln(chr(201));
	gotoxy(x+56,y-11); writeln(chr(203));
	gotoxy(x+72,y-11); writeln(chr(187));
	for i:=1 to 71 do
	begin
		if x + i  <> 60 then begin
		gotoxy(x+i,y); writeln(chr(205));
		gotoxy(x+i,y-11); writeln(chr(205));
		end;
	end;
	gotoxy(4,19); writeln('"Q" - Produce:  ');
	gotoxy(4,20); writeln('"W" - Consumer: ');
	textcolor(12);
	gotoxy(36,19); writeln('Buffer');
	textcolor(7);
	Put(20,19,11,dP_1);
	Put(20,20,7,dC_1);
	gotoxy(58,19); writeln('"E"');
	gotoxy(58,20); writeln('"R"');
	textcolor(7);
	Put(62,19,11,dP_2);
	Put(62,20,7,dC_2);
	gotoxy(4,6); writeln('"T" - Ticker:');
	Put(18,6,11,p1);
	gotoxy(41,6); write('"Y" - ');
	textcolor(14);
	write(chr(14));
	gotoxy(4, 24);
	write('Press ESC to stop');
	Put(49,6,11,p2);
	textcolor(12);
	gotoxy(15, 2);
	write('Press "Q", "W", "E", "R", "T", "Y" for speed control');
end;

procedure BufferFull;				{Вывод содержимого буферов процессов 1 и 2}
begin
	window(1,1,80,25);
	gotoxy(x, y);
	textbackground(10);
	textcolor(4);
	if x = 75 then 
		write(CountElem2 / n * 100:3:0, ' %')
	else
		write(CountElem1 / n * 100:4:0, ' % ');
	textbackground(0);
	textcolor(7);
end;

procedure Condition;				{Отображение состояния каждого процесса}
begin
	window(1,1,80,25);
	gotoxy(x-1,y);
	case n of
		1: begin textcolor(15); write(' ACTIVE'); end;
		2: begin textcolor(3);  write(' AWAIT '); end;
		3: begin textcolor(5);  write(' STOP  '); end;
	end;
	textcolor(7);
end;

procedure Put;						{Интерфейс визуализации состояния}
var									{скорости для каждого потока	 }
	i, nd : Byte;
begin
	window(1,1,80,25);
	gotoxy(x,y);
	if y = 6 then nd := 3 else nd := 4;
	for i:=0 to nd do 
	begin 
		if i = p then 
			textbackground(14)
		else 
			textbackground(col);
		write(' ');
	end;
	textbackground(0);
end;

procedure EndProc;					{Считывание оставшихся элементов }
begin								{из буферов и установка состояния}
	StatusCons1 := true;			{останова всех процессов	 }
	Consumer_1; Condition(26,20,3);
	
	StatusCons2 := true;
	Consumer_2; Condition(68,20,3);
	
	nosound;
	
	Condition(26,19,3);
	Condition(68,19,3);
	
	Condition(23,6,3);
	Condition(54,6,3);
	
	gotoxy(19,24);
end;

begin							{Переменные для управления и контроля	}
	ExitCode	:= 0;				{за состоянием каждого потока		}
	DT			:= 10;
	DT1			:= 3;
	DT2			:= 3;
	p1			:= 3;
	p2			:= 3;
	Timer		:= 0;
	Head1		:= 1;
	Teil1		:= 1;
	Head2		:= 1;
	Teil2		:= 1;
	CountElem1	:= 0;
	CountElem2	:= 0;
	dP_1		:= 2;
	dC_1		:= 3;
	dP_2		:= 1;
	dC_2		:= 2;	
	CountCh		:= 0;
	xP1			:= 0;
	yP1			:= 0;
	xP2			:= 0;
	yP2			:= 0;
	xP3			:= 1;
	yP3			:= 4;
	SimpleNum	:= 1;
	Position	:= 1;
	Note		:= 0;
	StatusProd1	:= false;
	StatusCons1	:= false;
	StatusProd2	:= false;
	StatusCons2	:= false;
	StopProd2	:= false;
	StopCons2	:= false;
	AwaitProd2	:= false;
	AwaitCons2	:= false;

	FileName := ParamStr(1);					
	assign(f, FileName);							{Инициализация файловой переменной}
	{$I-} reset(f); {$I+}							{Проверка файла на существование  }
	If IOResult<>0 then 
	begin 
		writeln(#10,' No file: ', FileName);
		halt;
	end;

	val(ParamStr(2), TickerPos, C2);
	if (C2 <> 0) or (TickerPos > 2) or (TickerPos < 1) 
		then begin 
		writeln(
		        #10,' Expected: curs0.exe [1] [2]',	{Проверка ввода параметров}
				#10#13,' [1] data.txt;',
				#10#13,' [2] Ticker position (1-top, 2-down)'); 
				halt; 
		end;
		
	if TickerPos=1 then yP3:=4 else yP3:=22;		{Положение бегущей строки}
	
	Ramka(4,18);
	
	reset(f);								{Открываем файл для чтения		}
	fSize := filesize(f);					{Записываем количество символов в f	}
	
	GetIntVec($8, @OldPOP1);
	SetIntVec($8, Addr(NewPOP1));
	GetIntVec($9, @OldPOP2);
	SetIntVec($9, Addr(NewPOP2));
	
	randomize;
	repeat 
		if StatusProd1 = true then 
			Produce_1						
		else if StatusCons1 = true then 
			Consumer_1
		else if StatusProd2 = true then 
			Produce_2
		else if StatusCons2 = true then 
			Consumer_2;
	until ExitCode = 2;					{Бесконечный цикл пока не нажата ESC}

	EndProc;
	close(f);							{Закрываем файл}
end.
