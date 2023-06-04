library("rpart")      # Библиотека для построения классификационного дерева
library("rpart.plot") # Библиотека для отображения дерева

Data = read.table("data_tree.txt", header=TRUE) # Читаем данные из файла

CTR = rpart.control(minsplit = 2)  # Минимальное количество наблюдений, 
                                   # которое должно существовать в узле, 
                                   # чтобы была предпринята попытка разделения
                                    
Tree = rpart(Class~X+Y,            # формула
             data=Data,            # данные
             method = "class",     # дерево классификации
             control = CTR)        # параметры подгонки

rpart.plot(Tree,                   # данные
           type=4,                 # тип построения
           branch=0,               # тип ответвлений
           extra=2,                # отображение доп. инфо
           roundint=FALSE,         # не округлять предиктор
           clip.right.labs=FALSE)  # метки в обоих поддеревьях

Tree                               # разделение вершин и прогнозируемый класс

newdata = read.table("data_test.txt", header=TRUE) # контрольный объект

predict(Tree, newdata) # предсказание класса контрольного объекта
