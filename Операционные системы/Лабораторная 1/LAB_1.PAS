{$M 1024, 0, 0}
uses Crt, Dos;

var
	OldPOP	 	 	: procedure;
	Code,Key		: byte;
	on_sound	 	: boolean;
	Hertz, Time	 	: integer;
	UpHertz, UpTime 	: integer;
	Cod1, Cod2	 	: integer;
	
function CheckSound : byte; { Проверка нажатия управляющих клавиш }
begin
	asm
		mov ah, 02h			{ Анализ флагов клавиатуры. 	}
		int 16h				{ В AL загрузка состояния клавиатуры. }
		and al, 00000110b		{ Проверка нажатия Shift Left + Ctrl. }
		cmp al, 6h			{ Если не нажаты, 			}
		jnz @@Null			{ функция возвращает 0, 		}
		mov @Result, 1			{ иначе 1. 				}
		jmp @@Exit
	@@Null:
		mov @Result, 0
	@@Exit:
	end
end;

{$F+}
procedure NewPOP; interrupt; 				{ Новая процедура обработки прерывания  }
begin

	Key := CheckSound;				{ Проверка нажатия Shift Left + Ctrl	 }
	Code := port[$60];				{ Получаем скан-код клавиши из порта $60}
		
	if (Key=1) and  (on_sound=false)then	 { Включаем озвучку если условие верно	 }
		on_sound := true
	else if (Key=1) and  (on_sound=true)then { Включаем озвучку если условие не верно}
		on_sound := false;

	if (Code > 128) then UpHertz := 2 else UpHertz := 1;	{ Если клавиша отпущена }
								{ увеличиваем звук		}
	{ Проверка нажати и отпускания клавиш Esc и Enter }
	{ если условие верно, то увеличиваем время. 	  }
	if (Code = 28) or (Code = 156) or (Code = 1) or (Code = 129)
	then UpTime := 5 else UpTime := 1;
	
	if on_sound=true then					{ Озвучиваем нажатия если условие верно }
	begin
		sound(Hertz * UpHertz);
		delay(Time * UpTime);
		nosound;
	end;
	
	inline($9C);		{ Помещение регистра флагов в стек }
	OldPOP;			{ Вызов оригинальной ПОП		}
end;
{$F-}

begin
	val(ParamStr(1), Hertz, Cod1);	{ Преобразование входных параметров }
	val(ParamStr(2), Time, Cod2);	{ с проверкой на тип. 			}
	
	if (Cod1<>0) or (Cod2<>0) or (Hertz<=0) or (Time<=0)then
	begin
		writeln(#10,' Error: Enter 2 parameters of Type Integer Above zero!');
		halt;
	end;
	writeln(#10,' Press Shift Left + Ctrl for UP or DOWN sound');
	
	on_sound := false;
	GetIntVec($9, @OldPOP);		 		{ Получаем адрес прерывания в OldPOP. 		}
	SetIntVec($9, Addr(NewPOP)); 			{ Устанавливаем новый адрес прерывания как NewPOP.}
	Keep(0);					 { Сохраняем программу в памяти как резидентную.   }
end.

