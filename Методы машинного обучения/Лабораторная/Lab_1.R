library("foreign")
library("dplyr")
library("ggplot2")
library("GGally")

Data = read.table("data.txt", header=TRUE)      # Читаем данные из файла в data.frame
DataD = data.frame(Data[3], Data[2]*Data[4])    # data.frame для отображаемых данных
colnames(DataD) = c('Мощность, л/с','Время разгона до max скорости, сек')
plot(DataD)                                     # Рисуем исходные данные и выбираем 
                                                # степень полинома для модели - 6
                                                # Структура данных для построения модели
DataF = data.frame(Data[2]*Data[4], Data[3], Data[3]^2, 
                   Data[3]^3, Data[3]^4, Data[3]^5, Data[3]^6)

colnames(DataF)=c('Y','X','X2','X3','X4','X5','X6')   # Задаем названия столбцам
model = lm(Y~X+X2+X3+X4+X5+X6, DataF)                 # Строим модель линейной регрессии

X0 = seq(25,300,by=5)                           # Значения независимого параматра (мощность)
Xpred=data.frame(X0,X0^2,X0^3,X0^4,X0^5,X0^6)   # Значения X0 для предсказания
colnames(Xpred)=c('X','X2','X3','X4','X5','X6') # Задаем названия столбцам
Y0=predict(model, Xpred)                        # Прогнозируемые значения на основе линейной модели объекта

lines(X0, Y0, col='red')                        # Рисуем кривую предсказания

Rezult=data.frame(Xpred[1], Y0)
colnames(Rezult)=c('Мощность:','Время разгона:')  
Rezult                                          # Выводим таблицу результатов прогноза
