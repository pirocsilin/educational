{$M 1024, 0, 0}

uses Crt, Dos;

const Impuls = 18.2;

var
	OldPOP				:	procedure;
	Cod1, Cod2, Cod3, NTick, nd	:	integer;
	GetCounter			:	longint absolute $0000:$046C;
	StartTick, StartAlarm		:	Real;
	EndAlarm, Alarm			:	Real;

procedure Tick;
begin
	if GetCounter >= StartTick then		{ Включаем тиканье если текущее 	  }
		if (GetCounter mod 6 = 0) then	{ значение счетчика >= StartTick.	  }
		begin				{ На время звонка будильника вызов 	  }
			sound(1500);		{ процедуры прекращаем и возобновляем	  }
			delay(10);		{ по завершении звонка будильника.	  }
			nosound;
		end;
end;

procedure TimeAlarm;				{ Включаем будильник если текущее	}
begin						{ значение счетчика >= StartAlarm. 	}
	sound(1500); delay(4); nosound;		{ Вызов процедуры продолжаем пока  	}
	sound(1200); delay(3); nosound;		{ значение счетчика не будет <= EndAlarm.}
	sound(1700); delay(4); nosound;
	sound(1600); delay(2); nosound;
end;

{$F+}
procedure NewPOP; interrupt;
begin
	inline($9C);
	OldPOP;
	
	if (GetCounter >= StartTick) and (GetCounter < StartAlarm) then 
		Tick
	else if (GetCounter >= StartAlarm) and (GetCounter <= EndAlarm)then
		TimeAlarm
	else if GetCounter > EndAlarm then
		Tick;
end;
{$F-}

begin
	val(ParamStr(1), NTick, Cod1);				{ Получение входных параметров }
	val(ParamStr(2), Alarm, Cod2);				{ с проверкой на тип. 		}
	val(ParamStr(3), nd, Cod3);
	
	StartTick  := NTick  * Impuls + GetCounter;	 	{Установка времени старта тиканья.}
	StartAlarm := Alarm  * 60 * Impuls + GetCounter; 	{Установка времени начала звонка. }	
	EndAlarm   := nd  	 * Impuls + StartAlarm;	 	{Продолжительность звонка.	}

	if (Cod1<>0) or (Cod2<>0) or (Cod3<>0) or (NTick>=Alarm*60) 
							  or (nd<=0) or (NTick<=0) or (Alarm<=0) then
	begin
		writeln(#10,' Enter 3 parameters:', #10, #13,
					'   1. start Ticks in seconds (Integer type)', #10, #13,
					'   2. start Alarm in minutes (Real    type)', #10, #13,
					'   3. stop  Alarm in seconds (Integer type)', #10, #13,
					' RULES:', #10, #13, 
					'   1) start Alarm > start Ticks', #10, #13,
					'   2) All parameters must be greater than zero!');
		halt;
	end;
	
	GetIntVec($8, @OldPOP);		 { Получаем адрес прерывания в OldPOP. 		}
	SetIntVec($8, Addr(NewPOP)); 	 { Устанавливаем новый адрес прерывания как NewPOP.}
	Keep(0);			 { Сохраняем программу в памяти как резидентную.   }
end.
