# QtSamples
Many small samples of qt projects


http://qt.shoutwiki.com/wiki/Closing_a_Qt_Quick_application_from_QML


### QtSamples
Test


### WIX

https://stackoverflow.com/questions/36671672/how-to-enable-wix-projects-in-visual-studio-2017

https://marketplace.visualstudio.com/items?itemName=RobMensching.WixToolsetVisualStudio2017Extension

https://github.com/wixtoolset/wix3/releases/tag/wix3104rtm

# Почему не стоит использовать qobject_cast<MyClass*>(sender())

- Этот метод помечен qt как deprecated
- В многопоточных системах с sender() есть проблема:
sender() - вовзвращает отправителя - Последний испустивший сигнал объект
И если повезёт, то sender() - вернёт нам того объекта отправителя сигнала какого нам надо
А если не повезёт, то sender() - вернёт нам последнего, который будет СОВСЕМ НЕ тот кого мы ожидаем
```
MyClass *senderMy   = qobject_cast<MyClass*>(sender());
bool    isSenderMy = (senderMy != nullptr);
```

# Шаблоны и Qt / Q_OBJECT + template<class T> 

https://dolzhenko.blogspot.com/2008/10/qt-usage-c-templates-with-qobject.html
	
**PROBLEM**: Using the Meta-Object Compiler (moc):
moc doesn't create moc class (qt meta info for class) for follow class declaration:
```
template <class T>
class SomeWidget : public SomeSubWidget {
 Q_OBJECT
public:
 SomeWidget(QWidget *parent = 0): SomeSubWidget(parent){
 }
 // for instance, wrap T class
protected:
 virtual T* component(QWidget *parent = 0) { return new T(parent); }

 T *widget;
};
```


**Solution**: devide class for two classes - with Q_OBJECT declaration and with template:
create sub class with Q_OBJECT
```
class SomeSubWidget : public QWidget
{
 Q_OBJECT
public:
 SomeSubWidget(QWidget *parent = 0):QWidget(parent){}
};
```

and when inherit it and add template:
```
template <class T> class SomeWidget : public SomeSubWidget
{
public:
 SomeWidget(QWidget *parent = 0): SomeSubWidget(parent) { } // for instance, wrap T class
protected:
 virtual T* component(QWidget *parent = 0) { return new T(parent); }
 T *widget;
};
```

https://stackoverflow.com/questions/4397478/qt-templated-q-object-class

**PROBLEM**: It is not possible to mix template and Q_OBJECT 

**SOLUTION 2**: If you have a subset of types you can list the slots and signals like this:
```
class SignalsSlots : public QObject
{
  Q_OBJECT
  public:
    explicit SignalsSlots(QObject *parent = 0) :
    QObject(parent) { }
  public slots:
    virtual void writeAsync(int value) { }
  signals:
    void readAsynkPolledChanged(int value);
};
...
template <class T>
class Abstraction : public SignalsSlots
{...
```
	
**Pimpl** 
https://stackoverflow.com/questions/12294981/interfaces-with-template-methods


# QScroller::grabGesture + QScrollerProperties
- QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
- Approach with QScrollerProperties using the scroller obj.
- setTableScrollerNonDragableProperties<QTreeView>(ui->treeViewTable);

```
#include <QTreeView>
#include <QScroller>
#include <QScrollerProperties>

template <class H>
void setTableScrollerNonDragableProperties(H *table)
{
    QScroller *scrol = QScroller::scroller(table);
    // (*) What this part of code does:
    // When you are at the top and scroll down realy fast on the table with your finger gesture
    // When bable comes to the end it will Drag (move a bit under the visible bottom and return to the top again)
    // To make the table just move down to the end and then simply Stop without dragging and stop at the bottom
    // we prepare QScrollerProperties scrprop; with following parameters:

    QScrollerProperties scrprop;
    QVariant overshootPolicy = QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff);
    scrprop.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, overshootPolicy);
    scrprop.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, overshootPolicy);
    scrprop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    scrprop.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);

    scrol->setScrollerProperties(scrprop);

    scrol->grabGesture(table, QScroller::LeftMouseButtonGesture);
}

```

# QString s; s.arg().arg()... <=> QStringList

```
QStringList listProxy;
listProxy.append("Z");
listProxy.append("X");
listProxy.append("Y");
QString funcProxy = "a'%1'b'%2'c'%3'"; // a'Z'b'X'c'Y'
for (int i = 0; i < listProxy.size(); ++i)
    funcProxy.replace(QString("%%1").arg(i + 1), listProxy.at(i));
```

# QMessageBox

### QMessageBox BAD:
```
QMessageBox msg;
    msg.setWindowTitle(tr("Invalid variable value"));
    msg.setText(tr("Unable to convert Variant value to int state"));
    msg.exec();
```

https://doc.qt.io/qt-5/qdialog.html#exec

### QMessageBox GOOD:
```
QMessageBox::warning(nullptr, "Warning", "Macros recording wasn't stopped!\n Stop recording the macros and try again.", "Ok");
QMessageBox::critical(this, "Error", error);
QMessageBox::about(this, tr("About program"), aboutMessage);

```

# qstring arg multiple times in a circle

	QString str;

	// Version 1
	str = "some %5987%239 random %66 text";
	for(int i=0; i<3; ++i)
	{
		str = str.arg( QString::number( (i+1)*10 ) );
		// returns "some 1020 random 30 text"
	}
	
	// Version 2
	str = "some %1 random %2 text %3 a %3 b %1 c %2 d finish";
	for(int i=0; i<3; ++i)
	{
		QString argNum = "%"+QString::number(i+1);
		str = str.arg(argNum).arg( QString::number( (i+1)*10 ) );
		// returns "some 10 random 20 text 30 a 30 b 10 c 20 d finish"
	}


# tr("your cool text") => ???? ?????? ?????

PROBLEM: Sometimes we get **???? ???? ???** on the place of our translated text.
Mostly It happends when we translate from English to Any Unicode Language.

setText(QCoreApplication::translate("Text", text.toLocal8Bit()));

### toLocal8Bit() can cause this problem

Use instead of toLocal8Bit() <---> toUtf8()


# QFile .open() correct error handling file.errorString()

```
QString flName;
QFile fl(flName); // exists / permission denied / other process busy
QString error;
if(fl.exists())
{
    if(fl.open(QIODevice::ReadOnly))
    {
        QByteArray block = fl.read(10);
        fl.write(block);
        fl.close();
    }
    else
    {
        error = QString("File: %1 Error: %2").arg(flName).arg(fl.errorString());
    }
}
else
{
    error = "File does not exist";
}

```


# QDir create for DEBUG

```
QDateTime dt = QDateTime::currentDateTime();
QDir dr; dr.mkdir("C:/Users/Public/test/" + dt.toString("dd-MM-yyyy-hh-mm-ss-z"));
```

# QFile create for DEBUG

```
{
	QDateTime dt = QDateTime::currentDateTime();
	QString sdt = dt.toString("dd_MM_yyyy_hh_mm_ss_z");
	sdt = sdt.append(".txt");

	QString file = "C:/test/_/" + sdt;

	writeStrToFile("1", file);
	writeStrToFile("2", file);
	writeStrToFile("3", file);
}

void writeStrToFile(QString scriptTetx, QString fileName)
{
	std::string s = fileName.toStdString();

	QFile scriptFile(fileName);
	if (scriptFile.open(QIODevice::ReadWrite))
	{
		// Save the script
		QTextStream outputStream(&scriptFile);
		QString code = scriptTetx;
		code = code.append("\n");
		outputStream.seek(outputStream.readAll().size());
		outputStream << code;
		scriptFile.close();
	}
}
```

# QJSEngine - setObjectOwnership

```
	QJSEngine _scriptEngine(this);
	// Register objects for access from script
	QJSValue svApi = _scriptEngine.newQObject(this);
	_scriptEngine.globalObject().setProperty("api", svApi);

	// all cpp objects that were passed to QJSEngine or QQmlEngine will be destroy by context of these object.
	// Without this call destuctor of ScriptInterface will be called 2 times - by EmulatorUiApplication and ScriptEngine;
	QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
```

# QString - smal guide
	
```
    QString s("1234567890");
    s = s.mid(3, s.length() - 3); // all after first 3
    s = s.left(3); // first 3
    s = s.right(3); // last 3
    
```

### SELECT BIG Data from a Large Table CORREC Solution:

```
    //
    // canFetchMore() + fetchMore() approach
    //
    
    while (mModel->canFetchMore())
        mModel->fetchMore();
    int totalRows = mModel->rowCount();
    
    for (int row = 0; row < totalRows; ++row) {
        for (int col = 0; col < mModel.columnCount(); ++col) {
            out << mModel->index(row, col).data().toString() << mFieldDelimeter;
        }
        out << EOL;
    }

```

### SELECT BIG Data from a Large Table BAD Solution:

```
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


	int rows = model->rowCount();
        int cols = model->columnCount();
        bool modelShowsAllRecords = !mModel->canFetchMore();
        
        if (modelShowsAllRecords) {
            for (int row = 0; row < rowCnt; ++row) {
                for (int col = 0; col < colCnt; ++col) {
                    out << mModel->index(row, col).data().toString() << mFieldDelimeter;
                }
                out << EOL;
            }
        }
        else {
            QSqlQuery query(mModel->query());
            bool      table_ok = query.exec();

            if (!table_ok) {
                QString sqlError("Error from SystemEventLogTable: " + query.lastError().text());
                qDebug() << sqlError;
                return ExportError(QString("Export to file error: %1").arg(sqlError));
            }
            else {
                // Only way to get a row count, size() method in QSqlQuery class
                // does not work for SQLite3
                query.last();
                int rowFullCount = query.at() + 1;
                if (rowCnt != rowFullCount)
                    rowCnt = rowFullCount;
            }

            if (rowCnt != 0) {
                query.first(); // return to first row
                do {
                    for (int col = 0; col < colCnt; ++col) {
                        out << query.value(col).toString() << mFieldDelimeter;
                    }
                    out << EOL;
                } while (query.next());
            }
        }

        return Result<void>::ok();
```
	

### OLD find function version - NO ALGO used


```
findTrainNumberNumberInContainer( QVector<StationScheduleInfo>& vec, int trainNumber, int& countStops)
{
	bool isNumberFound = false;
	for (auto a : vec)
	{
		if (countStops > 0 && !isNumberFound)
			return true;

		isNumberFound = (a.trainNum() == trainNumber);
		if (isNumberFound)
			countStops++;
	}

	return false;
}

```

## NEW find function version - USING  ALGORUTHM

```
	countStops = std::count_if(vec.begin(), vec.end(), [&](StationScheduleInfo elem) { return elem.trainNum() == trainNumber; });
	return countStops > 0;
```


### Center Coordinates for dialog
```
void GetCenterCoordsForDlg(QRect dlgLoginSize)
{
	// QListIterator<QScreen*> it(qApp->screens());
	// QScreen* firstScreen = qApp->screens().at(0);
	// QRect rct = (firstScreen)->geometry();
	// int sumWidth = rct.width();
	// while (it.hasNext())
	// {
	// 	QScreen *singleScreen = it.next();
	// 	sumWidth += singleScreen->geometry().width();
	// }

	QRect resultRect;

	QRect mainWindowRect = this->geometry();
	QListIterator<QScreen*> it(qApp->screens());
	QScreen* firstScreen = qApp->screens().at(0);
	QRect rct = (firstScreen)->geometry();
	int sumWidth = rct.width();
	// Show login dialog upon top-left x coordinate
	//int appWindowXposition = this->x();
	// Show login dialog upon coordinates of the center of the application window
	int appWindowXposition = this->x() + this->width() / 2;
	while (it.hasNext())
	{
		QScreen *singleScreen = it.next();
		QRect screenRect = singleScreen->geometry();
		int lastFoundScreenWidth = screenRect.width();

		if (appWindowXposition < sumWidth)
		{
			auto height = qMin(dlgLoginSize.height(), screenRect.height());
			auto width = qMin(dlgLoginSize.width(), screenRect.width());
			auto x = (screenRect.width() - width) / 2;	// center
			x += (sumWidth - lastFoundScreenWidth);		// center + step to right on all of the screens that we have met before
			auto y = (screenRect.height() - height) / 2;

			resultRect = QRect(x, y, width, height);
			qApp->sendCenterCoordsForDlg(resultRect);
		}
		else
		{
			sumWidth += screenRect.width();
		}
	}

}
```

# BEFORE REVIEW

```

QVariant StationScheduleTableModel::data(const QModelIndex & index, int role) const
{
  if (index.isValid())
  {
    int irow = index.row();
    int cnt = _itemsShown.count();

    if (irow < 0 || irow >= cnt)
    	return QVariant();

    const StationScheduleInfo &item = _itemsShown[irow];

    switch (role)
    {
    case TrainNumberRole:
    {
    	if (irow % _distanceNum == 0)
    		return item.trainNum();
    }
    break;
    case StationIdRole:
    	return item.stationId();
    case StationNameRole:
    	return item.stationName();
    case ArivalTimeRole:
    	return item.arivalTimeString();
    case DepartureTimeRole:
    	return item.departureTimeString();
    default:
    	return QVariant();
    }
  }

  return QVariant();
}

```

# AFTER REVIEW

```
  QVariant result;
  if (index.isValid())
  {
    int irow = index.row();
    int cnt = _itemsShown.count();

    if (irow < 0 || irow >= cnt)
    	return QVariant();

    const StationScheduleInfo &item = _itemsShown[irow];

    switch (role)
    {
    case TrainNumberRole:
    {
    	if (irow % _distanceNum == 0)
    		result = item.trainNum();
    }
    break;
    case StationIdRole:
    	result = item.stationId();
    	break;
    case StationNameRole:
    	result = item.stationName();
    	break;
    case ArivalTimeRole:
    	result = item.arivalTimeString();
    	break;
    case DepartureTimeRole:
    	result = item.departureTimeString();
    	break;
    default: break;
    }
  }

  return result;
```

```
{
    connect(ui->slider, &QSlider::valueChanged, this, &A::onSetValue);
    connect(ui->stepLeftButton, &QPushButton::clicked, this, &A::onMoveStepLeft);
    connect(ui->stepRightButton, &QPushButton::clicked, this, &A::onMoveStepRight);
    ui->slider->setValue(settings->value());
    ui->label->setText(QString::number(settings->laserValue()));

    ui->slider->setSingleStep(STEP);
    ui->slider->setPageStep(STEP);
    ui->slider->setTickInterval(STEP);
}
void A::onSetValue(int v)
{
    int res = std::round(v / STEP) * STEP;
    ui->slider->setValue(res);
    settings->setValue(res);
    ui->label->setText(QString::number(res));
}
void A::onMoveStepLeft()
{
    doMoveStep(true);
}
void A::onMoveStepRight()
{
    doMoveStep(false);
}

void A:doMoveStep(bool isleft)
{
    int v   = ui->slider->value();
    auto mi = ui->slider->minimum();
    auto ma = ui->slider->maximum();
    auto s  = STEP;

    for (int i = mi, l, r; i < ma; i += s) {
        l  = i;
        r = l + s;
        if (v >= l && v <= r) {
            if (l == v && v >= m) {
                v = isleft ? (l - s) : (l + s);
                break;
            }
            else if (r == v && v <= ma) {
                v = isLeft ? (r - s) : (r + s);
                break;
            }
            else if (v > l && v < r) {
                val = isleft ? l : r;
                break;
            }
        }
    }
    ui->slider->setValue(v);
}
```


# **Разработка веб-приложений с использованием QML и Qt для WebAssembly**

 **Установка и настройка.**

Нужно сделать две вещи: настроить компилятор Emscripten и Qt для веб-сборки.

**Emscripten**

на Linux:

- `# fetch Emscripten SDK and install target version`
- `git clone https://github.com/emscripten-core/emsdk.git`
- `cd emsdk`
- `./emsdk install sdk-1.38.30-64bit`
- `./emsdk activate --embedded sdk-1.38.30-64bit`
- `source emsdk_env.sh`

Обратите внимание, что 1.38.30 рекомендуется для сборки, не экспериментируйте с другими сборками, так как у них будут проблемы.

Исходный код `emsdk_env.sh` важен для настройки правильной среды *Emscripten*, поэтому не забывайте запускать ее <u>(каждый раз, когда вы открываете новый терминал</u>)


**Qt WebAssembly**

Это еще проще. Загрузите установщик [Qt со страницы загрузки Qt](https://www.qt.io/download) или откройте существующий [инструмент Qt Maintenance,](https://doc.qt.io/qt-5/gettingstarted.html#updating-or-removing-qt) если он у вас уже установлен.

В обоих случаях вам просто нужно выбрать Qt для WebAssembly, в версии Qt  от 5.13.2.

 

![https://miro.medium.com/max/1633/1*XF9KMNHHaFMSjoUS6FbNpA.jpeg](file:///C:/Users/IGOR~1.SHA/AppData/Local/Temp/msohtmlclip1/01/clip_image004.jpg)

Установка Qt WebAssembly



# Привет WebAssembly

Перейдите к вашему проекту и создайте `build `каталог.  Затем просто вызовите новый `qmake `с поддержкой WebAssembly:

- `cd /path-to-hello-webassembly-project` - преход в дирректорию проекта
- `mkdir build && cd build` - создание папки `build` и переход в неё
- `/path-to-hello-webassembly/5.13.2/wasm_32/bin/qmake ..` - компилирует проект
- `make`

После того, как это будет сделано, вы получите один из файлов в вашем каталоге сборки -  projectname.html. Чтобы запустить его в браузере, нам нужен, как обычно, веб-сервер. Вы можете использовать все, что захотите, для обслуживания этих статических файлов, но есть также удобный инструмент командной строки emrun, который вы можете использовать для этой цели, поскольку он уже доступен как часть среды Emscripten.

Итак, чтобы открыть наше приложение в браузере Firefox, можно сделать:

- `emrun --browser=firefox *.html`
 

**<u>Пример из dev-shaldunov:</u>**

1. Выполняем из /home/user/emsdk:

     `./emsdk activate --embadded sdk-1.38.30-64bit`

2. `source emsdk_env.sh`
3.  Создаем папку `build` в корне проекта: /mywebassembly/build:
4. `/opt/Qt/5.13.2/wasm_32/bin/qmake ..`
5. `make` 

6. Чтобы открыть наше приложение в браузере Firefox, можно сделать:

   `emrun --browser=firefox *.html`

   

   P.S.: Более подробное описание Вы можете найти на: https://itnext.io/developing-web-apps-using-qml-and-qt-for-webassembly-aa84453f2f61
   





https://tproger.ru/articles/move-semantics-and-rvalue/

===================================================================
### Для чего нужен стек ?
- Стек
- Стек представляет собой непрерывную область памяти, адресуемую регистрами ESP (указатель стека) и SS (селектор сегмента стека).
- Особенность стека заключается в том, что данные в него помещаются и извлекаются по принципу «первым вошел – последним вышел». 
- Данные помещаются в стек с помощью инструкции PUSH (заталкивание), а извлекаются по инструкции POP (вытаскивание).
Кроме явного доступа к стеку с помощью инструкций  PUSH и POP, стек автоматически используется процессором при 
выполнении инструкций вызова, возвратов, входа и выхода из процедур, а также при обработке прерываний.

Стек используют для разных целей:
- организации прерываний, вызовов и возвратов;
- временного хранения данных, когда под них нет смысла выделять фиксированные места в памяти;
- передачи и возвращения параметров при вызовах процедур.

**Стек и Куча разница**. Разница между Стеком и Кучей между Кучей и Стеком
1. В стеке распределение и освобождение выполняются центральным процессором и выполняется автоматически, тогда как в куче программист должен делать это вручную.
2. Обработка кадров кучи обходится дороже, чем обработка кадров стека.
3. Реализация стека сложна. В отличие от этого, реализация кучи проста.
4. Вызов функции в стеке занимает O (N) времени. В отличие от этого, требуется куча O (1) времени.
5. Реализация стека в основном страдает от нехватки памяти. Напротив, основной проблемой в куче является фрагментация.
6. Доступ к кадру стека проще, чем кучи, поскольку стек ограничен небольшой областью памяти и всегда попадает в кэш, но кадры кучи разбросаны по всей памяти, поэтому доступ к памяти может привести к большему количеству пропусков кеша.
		
| Сравнение		| стек					| куча
|-----------------------|---------------------------------------|----------------------------------------
| основной		| Память выделяется 			| Память выделяется в случайном порядке
|			| в (LIFO) режиме «посл.вх.перв.вых» 	| - 
| Распределение		| автоматическая 			| Руководство по эксплуатации и выделению
| Стоимость		| Меньше				| Больше
| Реализация		| Жесткий				| Легко
| Вызов фции		| О(N)					| O(1)
| Проблемы		| Ограниченная память			| Фрагментация памяти
| Место-дение ссылки	| Превосходно				| адекватный
| гибкость		| Фиксированный размер и не гибкий	| Изменение размера возможно
| Время доступа		| Быстрее				| Помедленнее
	
===================================================================
### Стек 1 на программу ?
Ответ: Программа это процесс -> у процесса может быть несколько потоков. У каждого потока свой СТЕК (1 юзер-мод стек + 2-ой Кернел мод стэк)
===================================================================

### Процесс от потока - разница ? (один есть основной поток)
- Процесс это контейнер для потоков.
- Несколько потоков пользуются общим адреснымс пространством процесса
===================================================================
### static_cast и dynamic_cast разница -> 
- dynamic_cast делает проверку на соотвествие типов
- static_cast НЕ делает проверку на соотвествие типов
===================================================================
### фабричный метод = виртуальный конструктор
===================================================================
### make_shared()
https://habr.com/ru/post/509004/
process(std::shared_ptr<Bar>(new Bar), foo());

- Плюс make_shared:
Для: std::shared_ptr<int> x(new int);

Выделений памяти фактически будет два:
- 1-ое в скобках конструктора shared_ptr-а, сам вызов "new int" для выделения памяти под объект
- 2-ое для выделения памяти под сам умный указатель, где оне будет храниить счётчик ссылок int count_copy; и Инкапсулированный указатель на объект Temp*ptr;

Для: std::shared_ptr<int> x(new int);
	
These functions will typically allocate more memory than sizeof(T) to allow for internal bookkeeping structures such as the reference counts

### 1. ПРОБЛЕМА: Может произойти утечка памяти, для случая, 
- СИТУАЦИЯ: когда  функция принимает как минимум два параметра, один из которых инициализируется свежесозданным безымянным std::shared_ptr, а второй параметр инициализируется вызовом другой функции, которая может бросать исключения. 
- СУТЬ: Компилятор не гарантирует порядок "вычисления" аргументов функции, он может перемешать и ЕСЛИ порядок такой - new Bar; foo(); конструктор std::shared_ptr, а не new Bar; к-р std::shared_ptr; foo(), при исключекнии  утечку экземпляра Bar.

А чтобы потенциальная утечка памяти реализовалась — надо ещё два условия:
- чтобы компилятор перемешал вызовы неблагоприятным образом;
- чтобы функция, вычисляющая второй параметр, действительно бросила исключение.

===================================================================

### Хеш таблицы - Разрешение коллизий (Бакеты (+в цепочках Entries ещё)в обих случаях юзаются)
1. **Метод цепочек** - Разрешение коллизий при помощи цепочек.
Каждая ячейка массива H является указателем на связный список (цепочку) пар ключ-значение, соответствующих 
одному и тому же хеш-значению ключа. Коллизии просто приводят к тому, что появляются цепочки длиной более одного элемента.
Операции поиска или удаления элемента требуют просмотра всех элементов соответствующей ему цепочки, 
чтобы найти в ней элемент с заданным ключом. Для добавления элемента нужно добавить элемент в 
конец или начало соответствующего списка, и, в случае, если коэффициент заполнения станет слишком велик, 
увеличить размер массива H и перестроить таблицу.
При предположении, что каждый элемент может попасть в любую позицию таблицы H с равной вероятностью 
и независимо от того, куда попал любой другой элемент, среднее время работы операции поиска элемента 
составляет Θ(1 + α), где α — коэффициент заполнения таблицы.
2. **Открытая адресация**
Пример хеш-таблицы с открытой адресацией и линейным пробированием, получающейся при вставке элементов в левой колонке сверху вниз.
В массиве H хранятся сами пары ключ-значение. Алгоритм вставки элемента проверяет ячейки 
массива H в некотором порядке до тех пор, пока не будет найдена первая свободная ячейка, 
в которую и будет записан новый элемент. Этот порядок вычисляется на лету, что позволяет 
сэкономить на памяти для указателей, требующихся в хеш-таблицах с цепочками.
Последовательность, в которой просматриваются ячейки хеш-таблицы, называется последовательностью проб.

===================================================================

### Лямбды:
В C++ 11 лямбда-выражение — это удобный способ определения объекта анонимной функции (замыкания) непосредственно в расположении, где оно вызывается или передается в качестве аргумента функции
```
- Хороши для // Хорошо: Мы можем хранить лямбду в именованной переменной и передавать её в функцию в качестве параметра
auto isEven{   [](int i) { return ((i % 2) == 0); } };
return std::all_of(array.begin(), array.end(), isEven);
```
```
- И примерно такого же аналога в Qt для функции connect -> connect(this, someSig, this, []{ slot-logic right away })
[ captureClause ] ( параметры ) -> возвращаемыйТип
{ стейтменты; }
Поля captureClause и параметры могут быть пустыми, если они не требуются программисту.
Поле возвращаемыйТип является опциональным, и, если его нет, то будет 
использоваться вывод типа с помощью ключевого слова auto.
```
- *"Замыкание"*
auto found{ std::find_if(arr.begin(), arr.end(), [search](std::string_view str) {
Поле capture clause используется для того, чтобы предоставить (косвенно) лямбде доступ к переменным из окружающей области видимости, 
к которым она обычно не имеет доступ. Всё, что нам нужно для этого сделать, так это перечислить в поле capture clause объекты, 
к которым мы хотим получить доступ внутри лямбды. В нашем примере мы хотим предоставить лямбде доступ к значению переменной search, поэтому добавляем её в захват
- *"Замыкание" / "Захват"* 
Ключевой момент: Переменные, захваченные лямбдой, являются клонами переменных из внешней области видимости, а не фактическими «внешними» переменными.
Для продвинутых читателей: Когда компилятор обнаруживает определение лямбды, он создает для нее определение как для пользовательского объекта. 
Каждая захваченная переменная становится элементом данных этого объекта. Во время выполнения программы, при обнаружении определения лямбды, создается экземпляр объекта лямбды и в этот момент инициализируются члены лямбды.
- *Захват по значению*
Чтобы разрешить изменения значения переменных, которые были захвачены по значению, мы можем пометить лямбду как mutable. 
В данном контексте, ключевое слово mutable удаляет спецификатор const со всех переменных, захваченных по значению:
- *Захват по ссылке*
Чтобы захватить переменную по ссылке, мы должны добавить знак амперсанда (&) к имени переменной, которую хотим захватить.
  - В отличие от переменных, которые захватываются по значению, переменные, которые захватываются по ссылке, не являются 
  - константными (если только переменная, которую они захватывают, не является изначально const)
  
===================================================================

- **override** - мы именно перегрузили метод, который был объявлен как virtual
- **finale** - нужен затем, чтобы никто, кто наследуется от этого класса не мог бы перегрузить его

===================================================================

```
МЕТОДЫ НЕ ВЛИЯЮТ НА РАЗМЕР ОБЪЕКТА !!! Ответ: 
Размер объекта в общем случае равен = (1) + (2) + (3) + (4) 
(1) + sizeof всех членов класса 
(2) + остаток для выравнивания (по умолчанию выравнивание 4 байта) 
(3) + vtblptr = sizeof указателя на vtable (если есть виртуальные функции, 
который находится вскгда ПЕРВЫМ в памяти объетка, чтобы имея указатель на базовый класс 
ты в любом случае мог бы обратиться ко всем функциям (любой из на любой ступни наследования) ) 
(4) + указатели на классы предков, от которых было сделано виртуальное наследование 
(=размер указателя * количество классов, есть множ.насл. от двух классов, значит и будет 2 указат. на них) 
(0) - методы внутри объекта не влияют на размер объекта (static) - static-поля НЕ ЯВЛЯЮТСЯ ЧАСТЬЮ КЛАССА
```
===================================================================
### lvalue и rvalue
https://tproger.ru/articles/move-semantics-and-rvalue/
- lvalue - то, что имеет конкретный адрес в памяти
- rvalue - то, что адреса не имеет, то, что называется временным объектом
- lvalue в том, что объекты rvalue могут быть перемещены, тогда как объекты lvalue всегда копируются.

===================================================================
### null_ptr
Rengen, для обнуление указателя лучше использовать nullptr, т.к.:
"Причина введения nullptr в язык С++ заключается в устранение возможных недоразумений между (макросом NULL = 0) литералом 0 как целочисленным значением и указателем. Двойное значение литерала 0 может вызывать проблемы при некоторых обстоятельствах. Литерал nullptr имеет тип std::nullptr_t, и его нельзя перепутать со значением другого типа. Он может быть неявно преобразован в любой ссылочный тип, но ни в один целочисленный тип, кроме типа bool." - Айвор Хортон, Visual C++ 2010, Глава 4 *примечание.

Для C++ более старых версий nullptr легко добавляется одной строчкой:
C++Выделить код static struct nullptr_t {template<typename T> operator T* () const {return 0;}} nullptr;
	
### NULL
Страуструп описывал про макрос NULL:
- 5.1.1. Ноль.
В языке С было очень популярно определять макрос NULL для представления такого нулевого указателя. Так как в C++ типы проверяются более жестко, использование банального нуля вместо NULL приведет к меньшим проблемам. Если вы чувствуете, что просто обязаны определить NULL, воспользуйтесь
const int NULL = 0;

===================================================================
```
https://ravesli.com/urok-195-std-weak_ptr-tsiklicheskaya-zavisimost-s-std-shared_ptr/
https://eax.me/cpp-smart-pointers/
https://habr.com/ru/post/140222/
```
- **auto_ptr** - deprectaed, заменён unique_ptr-ром - реализует разрушающее копирование, тоесть имея 2 указателя и при передаче ссылки на объект из одного указателя в другой. Второй куда произошла передача станет "владельцем объекта", а Исходный указатель становится невалидным.
- **unique_ptr** - когда ресурс памяти не должен был разделяемым (у него нет конструктора копирования), но он может быть передан другому unique_ptr или владение им с помощью std::move. представляет собой уникальный указатель на объект. Указатель нельзя копировать, но можно передавать владение им с помощью std::move. При уничтожении указателя автоматически вызывается деструктор объекта, на который он указывает.
- **shared_ptr** - должен использоваться, когда ресурс памяти должен быть разделяемым. (переменная, которая хранит количество указателей, которые ссылаются на объект, Если эта переменная становится равной нулю, то объект уничтожается. Счетчик инкрементируется при каждом вызове либо оператора копирования либо оператора присваивания)
- **weak_ptr** - содержит ccылку на объект, которым управляет shared_ptr, но не производит подсчет числа ссылок; позволяет избавиться от циклической зависимости
- (!) Недостатком умного указателя std::weak_ptr является то, что его нельзя использовать напрямую (нет оператора ->). Чтобы использовать std::weak_ptr, вы сначала должны конвертировать его в std::shared_ptr (с помощью метода lock()), а затем уже использовать std::shared_ptr.
===================================================================

### std::move и std::forward - https://habr.com/ru/post/568306/

```
template<class T> class vector {
    // ...
	Конструктор
	Деструктор
    vector(const vector&);            // copy constructor - конструктор копии
    vector(vector&&) noexcept;        // move constructor - перемещающий конструктор копии
    vector& operator=(const vector&); // copy assignment - оператор присваивания
    vector& operator=(vector&&);      // move assignment - перемещающий оператор присваивания
  public:
  A();
  ~A();
  A(const& A _obj);
  const& A operator=(const& A _obj);
  A(const&& A _obj);
  const& A operator=(const&& A _obj); 	
};
```
- **std::move()** - не выполняет никаких перемещений - занимется ТОЛЬКО ПРИВЕДЕНИЕМ ЛЮБОГО ТИПА к типу rvalue-ссылки. Внутри него происходит
1. Удаление Всех ссылок из входного типа, и
2. Подставление "Двух ампесантов" соотвественно, чтобы получилась Rvalue-ссылка.
- **std::forward()** - фция нужная нам из-за особенности std::move - так как std::move безусловно приводит свой результат к rvalue, а мы хотели бы иметь возможность работкть так же и с lvalue, чтобы они передавались как lvalue
1. Если ссылка была передана как rvalue, то отрабатывает фактически std::move, и передача rvalue – происходит и далее как rvalue
2. И соотвественно если на вход получили lvalue ссылку, то и возвращаем так же lvalue ссылку (просто возвращаем то, что передали нам на вход).
```
template<typename T>
T&& forward(T&& param)
{
    if (is_lvalue_reference<T>::value)
        return param;
    else
        return move(param);
}
```

### **Склейка ссылок**

``` Если при разворачивании шалона, получается lval-ref на lval-ref => 1. T& & => T& 2. T& && => T& 3. T&& & => T& 4. T&& && => T&& ```

===================================================================
### Для чего нужен std::forward?
Функция std::forward, как известно, применяется при идеальной передаче (perfect forwarding).

Идеальная передача позволяет создавать функции-обертки, передающие параметры без каких-либо изменений (lvalue передаются как lvalue, а rvalue – как rvalue) 
и тут **std::move** нам не подходит, так как она безусловно приводит свой результат к rvalue.
Поэтому, была разработана функция **std::forward**, которая выполняет примерно следующую работу:
То есть, если ссылка была передана как rvalue, то вызываем духов std::move, а иначе просто возвращаем то, что передали.

1. Если необходимо «передать» умный указатель **std::unique_ptr**, то сделать мы можем это только через std::move
2. использование std:move можно найти в различных алгоритмах, где нужно менять элементы местами
3. Быстрый Переброс данных из вектора в вектор
```
v2.insert(v2.end(), std::make_move_iterator(v1.begin() + 7), 
                    std::make_move_iterator(v1.end()));
As others have pointed out, first vector v1 is now in indeterminate state, so use erase to clear the mess:
v1.erase(v1.begin() + 7, v1.end());
```
===================================================================
### explicit - https://habr.com/ru/post/436296/
- это ключевое слово ставится перед объявлением конструктора с одним параметром (или с большим числом параметров, 
но когда все параметры, начиная со второго, имеют значения по умолчанию) и предотвращает неявное преобразование типов при инициализации.
- С появлением с C++11 универсальной инициализации explicit обрёл смысл для конструкторов с 0,2,3 и более параметров:
- Помимо этого, начиная с C++11 ключевое слово explicit может также применяться к операторам преобразования типа, также запрещая их неявный вызов:
- В заключение хочется порекомендовать использовать универсальную инициализацию в любом новом коде на C++, а также явно объявлять конструкторы explicit всегда, кроме случаев, когда неявное преобразование семантически оправдано.
===================================================================
### placement new
- Как можно создать объект класса по определённому адресу в памяти ? по конкретному адресу в памяти ?
- Использовать оператор placement new
- Использовать MEMCPY ( Адрес_Куда, Адрес_Откуда, Размер_Скок_Скопировать ) 
``` memcpy( &dst[dstIdx], &src[srcIdx], numElementsToCopy * sizeof( Element ) ); ```
ещё как-то побитово копировать
===================================================================
### mutable 
- Для CONST-тантных методов, которые не могут изменять состояние экземпляра класса и которые и было введено ключевое слово mutable - ибо их он изменять может в отличие от всех сотальных.
- Самый релевантный пример его использования в реальной практике это mutable mutex внутри констатных Get-методов класса, которые должны обратиться к некому общему ресурсу внутри класса.
===================================================================
### static 
- static-методы класса имеют доступ только к статическим полям класса, т.к.
- static-методы не имеют прямой связи с конкретным экземпляром класса
и по этой причине static метод в классе не имеет доступа к указателю this
- inline функции не могут быть помечены как static
===================================================================
### inline
- Это такая функция, чье тело подставляется в каждую точку вызова, вместо того, чтобы генерировать код вызова. Нам не всегда гарантируется её встраиваемость:
- !!! ЕСТЬ ОГРАНИЧЕНИЯ:
- Некоторые компиляторы не могут компилировать встроенный код для функции, содержащей циклы, а также предложения switch и goto.
- Зачастую бывает нельзя создать рекурсивную inline функцию.
- Запрещается использование статических членов функции.
===================================================================
### Виртуальные методы из Конструкторов или Деструкторов

- НЕТ
Для случая создания объекта наследника, бызовется конструктор базового класса и далее начинаются проблемы:
- Т.к. конструктор Базового класса ещё не отработал полностью, а конструктор наследника вызовется только следующим за базовым, 
и таблица вирт.фций не проинициализирована полностью, то вызовется не функция наследника, вызов которой мы ожидаем, 
а вызовется функция Базового класса (если метод НЕ ЧИСТО виртуальный). В результате мы получим НЕОЖИДАЕМОЕ поведение.
- Если вызываемый метод будет ЧИСТО виртуальным (внутри конструктора) мы ещё и можем получить Исключение! **PURE VIRTUAL CALL EXCEPTION**

- Внутри Конструктора - не работает механизм виртуальных функций, потому что ещё не инициализирована таблица виртуальных функций.
- Внутри Деструктора - механизм виртуальных функций может сработать неправильно (не так как было задумано изначально), 
по причине того, что в таблице виртуальных функций будут содержаться указатели на методы уже уничтоженного класса наследника.

Подробнее: Базовый класс в нём определён конструктор с вызовом виртуальной функциии. Мы наследуемся, 
переопределяем в наследнике виртуальную функцию. При создании объекта наследника, будет сначала вызван 
конструктор Базового класса, в нём будет вызвана виртуальная фция переопределённая в наследнике, 
И ввиду того, что НАСЛЕДНИК НЕДОСФОРМИРОВАН (его конструктор ещё не отрабатывал) то как следствие возможна ошибка. 
Далее UB (undefined behavior) Вероятнее всего вызовется метод самого базового класса, если тот определён, иначе ... вопрос.

===================================================================
	
## PATTERN PATTERNS ШАБЛОНЫ ПРОЕКТИРОВАНИЯ СИСТЕМНЫЙ ДИЗАЙН ПАТТЕРНЫ
	
**CONS in general: OVERENGENEERING**

- **Abstract Factory** - Lets you produce families of related objects without specifying their concrete classes.
- **Builder** - Lets you construct complex objects step by step. The pattern allows you to produce different types and representations of an object using the same construction code.
- **Factory Method** - Provides an interface for creating objects in a superclass, but allows subclasses to alter the type of objects that will be created.

- **Singleton** – having only 1 instance of an object. – it’s OK if it’s not shared it’s perfect to get access from allover the place to this entity. A Singleton encapsulates a unique resource and makes it readily available throughout the application.	
- **Singleton** – shared resource [stored data] multiple objects must have access to that data, here we start to have race conditions (here we can use mutexes, cond-vars, double check pattern approach)	
- **Singleton** – extra problem – it can become a SUPERCLASS, that knows everything about everyone and every software part and software layer can have access to any variable or object that’s inside the Singleton (Laser-class).

- **Singleton** –  We can have any type of problems – starting from INCONSISTANCY OF VARIABLE STATES we read a buffered value, that has already changed it’s state on the device, finishing with – if it’s an shared resource and accessed from MULTIPLE THREADS at the same time, and it has a synchronization object we can get a deadlock
- **Facade** - Provides a simplified interface to a library, a framework, or any other complex set of classes.

Cons: Façade is usefull, when that simplified interface at the end really simplifies your life. We had a teammate who created 100 façade classes, each of them had a derived class, and our college sad when you will need the functionality of each of them, think in advance – Because 1) if you will be able to use my interface and subclass from it, then do it in the first place. 2) if you extremely need to subclass from the original class and you do not want to touch int interface, it’s up to you, but in this case think, maybe you are doing something wrong…
Strong CONNECTEDNESS, 

- **Template Method** - Defines the skeleton of an algorithm in the superclass but lets subclasses override specific steps of the algorithm without changing its structure.

- **State** - Lets an object alter its behavior when its internal state changes. It appears as if the object changed its class.,
- **Visitor** - Lets you separate algorithms from the objects on which they operate.
- **Strategy** - Lets you define a family of algorithms, put each of them into a separate class, and make their objects interchangeable.

- **Oberver or Listener** bei Client Server Applicationen
The Observer looks for sertain amount of Actions and has appropriate ActionHandlers classes, that are called when the action accures. 
_______________________->update() ------------------------------------ ObservA()
[Subject->Subjects changes]->update()] ObserverBase with notify() -> ObservB()
_______________________->update() --------------------------------------> ObservC()

- **Command** - Turns a request into a stand-alone object that contains all information about the request. This transformation lets you pass requests as a method arguments, delay or queue a request's execution, and support undoable operations.
- **Iterator** - Lets you traverse elements of a collection without exposing its underlying representation (list, stack, tree, etc.).
- **Proxy** - Lets you provide a substitute or placeholder for another object. A proxy contr- ols access to the original object, allowing you to perform something either before or after the request gets through to the original object.- **

- **Adapter** - Allows objects with incompatible interfaces to collaborate.
- **Composite** - Lets you compose objects into tree structures and then work with these structures as if they were individual objects.
- **Prototype** - Lets you copy existing objects without making your code dependent on their classes.
- **Decorator** - Lets you attach new behaviors to objects by placing these objects inside special wrapper objects that contain the behaviors.

