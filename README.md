# QtSamples
Many small samples of qt projects

TODO - https://www.youtube.com/watch?time_continue=347&v=iR8K6ya0ZaY&feature=emb_logo

http://qt.shoutwiki.com/wiki/Closing_a_Qt_Quick_application_from_QML

https://habr.com/ru/post/117996/
https://proglib.io/p/tricky-challenges-cpp
https://techrocks.ru/2019/03/01/popular-interview-questions-on-c-plus-plus/
https://pvs-studio.com/ru/blog/posts/cpp/0722/
https://medium.com/@rodrigues.b.nelson/choosing-wisely-c-containers-and-big-oh-complexity-64f9bd1e7e4c
https://flexiple.com/algorithms/big-o-notation-cheat-sheet/

https://www.cyberforum.ru/cpp-beginners/thread971708.html


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
   
# РАЗНОЕ	

### MVC MVP MVVP
https://www.youtube.com/watch?v=X85soC5evw0

- MVC - Model View Controller - Разделение Граф.Интерфейса - Управления - Бизнеслогики
    - Model - Повар (готовит люда, получает заказы, не знает про заказчика) - Бизнеслогика / Данные
    - Controller - Официант (не готовит, но приносит заказы и относит блюда) - Управление / Конкретная релазизация способа подготовки и передачи данных/команд в модель + получение и передача результата от модели и формировыание данных для Вьюхи 
    - View - Посетитель ресторана (заказывает блюдо, получсает блюдо, не зная о поваре и готовке) - Отображение данных пользователю и предоставление графического интерфейса для него
```
   Model
   ^  | Notify
   |  V 
 Controller
   ^  | Prepare view
   |  V 
   View
   ^  |
   |  V 
   User

```

### REST и SOAP разница
https://appmaster.io/ru/blog/mylo-protiv-otdykha (SOAP против REST: Ключевые различия)

- REST: НЕ Протокол, а архитектурный стиль
- SOAP: Кокнертный протокол

- REST: ТОЛЬКО HTTP протокол
- SOAP: Любой протокол HTTP, HTTPS, TCP, SMTP, и XMPP

- REST: Использует коды возврата HTTP протокола
- SOAP: Для результатов своих запросов так же формирует XML с ответами из кокнретного протокола

- REST: XML, JSON, Plain-Text, HTML
- SOAP: Только XML

- REST может кэшировать все вызовы, что делает его быстрее, чем SOAP.
- SOAP не может кэшировать вызовы, 

- REST: Нет чётких границ и нет строгости в принципах работы кроме реализации CRUD через методы HTTP
- SOAP: Есть строго описанная документация по протоколу, по работе с шифрованием, как организована безопасность, работа по сети и т.д.


# ООП

### Принципы SOLID

ЦЕЛЬ: При создании программных систем использование принципов SOLID способствует созданию такой системы, которую будет легко поддерживать и расширять в течение долгого времени. 

- S - SRP - (single responsibility principle) - S: Single Responsibility Principle (Принцип единственной ответственности). - Принцип единственной ответственности (###) - Каждый класс должен решать лишь одну задачу  - Для каждого класса должно быть определено единственное назначение. Все ресурсы, необходимые для его осуществления, должны быть инкапсулированы в этот класс и подчинены только этой задаче.

- O - OCP - (open-closed principle) Принцип открытости/закрытости (###) - «программные сущности … должны быть открыты для расширения, но закрыты для модификации».
O: Open-Closed Principle (Принцип открытости-закрытости). Программные сущности (классы, модули, функции) должны быть открыты для расширения, но не для модификации. (вместо switch-case makeSound, добавить виртуальный метод makeSound / интерфейс для класса)

- L - LSP - (Liskov substitution principle) -  Принцип подстановки Лисков (###) - «функции, которые используют базовый тип, должны иметь возможность использовать подтипы базового типа не зная об этом». См. также контрактное программирование. L: Liskov Substitution Principle (Принцип подстановки Барбары Лисков). Необходимо, чтобы подклассы могли бы служить заменой для своих суперклассов.  Цель этого принципа заключаются в том, чтобы классы-наследники могли бы использоваться вместо родительских классов, от которых они образованы, не нарушая работу программы. Если оказывается, что в коде проверяется тип класса, значит принцип подстановки нарушается

- I - ISP - (interface segregation principle) - Принцип разделения интерфейса (###) - «много интерфейсов, специально предназначенных для клиентов, лучше, чем один интерфейс общего назначения»[9]. I: Interface Segregation Principle (Принцип разделения интерфейса). Создавайте узкоспециализированные интерфейсы, предназначенные для конкретного клиента. Клиенты не должны зависеть от интерфейсов, которые они не используют.

- D - DIP - (dependency inversion principle) - Принцип инверсии зависимостей (###) - «Зависимость на Абстракциях. Нет зависимости на что-то конкретное»[9].
SOLID - https://medium.com/webbdev/solid-4ffc018077da D: Dependency Inversion Principle (Принцип инверсии зависимостей). Объектом зависимости должна быть абстракция, а не что-то конкретное.

Модули верхних уровней не должны зависеть от модулей нижних уровней. Оба типа модулей должны зависеть от абстракций.

Абстракции не должны зависеть от деталей. Детали должны зависеть от абстракций.

### POSA

### GRASP

### GOF
ГОФ - Gang Of Four - Банда четырех авторов, которые сделали. Книгу по основным паттерна проектирования, "Design Patterns: Elements of Reusable Object-Oriented Software

## YAGNI KISS DRY

### Принцип KISS – keep it simple stupid (делайте вещи проще)
### Принцип YAGNI - You ain’t gonna need it – вам это не понадобится
### Принцип DRY - Don't repeat yourself:
- Самый простой подход по уменьшению сложности — разделить систему на управляемые части.
- Принцип DRY требует, чтобы такие части информации встречались в вашем коде один, и только один раз.
- Каждая часть данных должна иметь четкое, надежное представление в системе.
- Если вдруг вы ловите себя на том, что этот код вы уже писали/встречали раньше, остановитесь, подумайте, и не повторяйте себя.
- DRY достигается совместным планированием.

### SFINAE
https://www.youtube.com/watch?v=DTpINiqvFqA&t=2440s

https://habr.com/ru/post/205772/

Аббревиатура **SFINAE** расшифровывается как **substitution failure is not an error** и означает следующее: при определении перегрузок функции ошибочные инстанциации шаблонов не вызывают ошибку компиляции, а отбрасываются из списка кандидатов на наиболее подходящую перегрузку. Выражаясь по-человечески, это означает вот что:
Когда речь заходит о SFINAE, это обязательно связано с перегрузкой функций.
- Это работает при автоматическом выводе типов шаблона (type deduction) по аргументам функции.
- Некоторые перегрузки могут отбрасываться в том случае, когда их невозможно инстанциировать из-за возникающей синтаксической ошибки; компиляция при этом продолжается как ни в чём не бывало, без ошибок.
- Отбросить могут только шаблон.
- SFINAE рассматривает только заголовок функции, ошибки в теле функции не будут пропущены.

### ADL 
- Argument-dependent lookup / Argument dependent lookup
- Позволяет нам не писать лишние имена неймспейсрв у функций begin и end, ибо мы используем их внутри std::sort, которая уже в std-namespac-e
```
std::sort( begin(vec), end(vec))
```

### Идиомы C++:

https://djvu.online/file/p3nXXMXDT9EJN

- Non-Copyable
- RAII
- Copy-and-swap
- Copy-on-write
- type_identity
- Type Traits
- Tag dispatch
- Policy-based design
- EBO
- Type Erasure
- SOO
- CRTP
- Barton-Nackman Trick
- Customization point object (Niebloid)
- Mixin
- Smart Pointer
- Singleton
- Erase-Remove

### Принципы ООП
1) Инкапсуляция - объект контролирует изменение своих свойств - это механизм языка, позволяющий **объекдинить данные и методы**, работающие с этими данными в единый объект, и **скрыть детали реализации** от пользователя при помощи **модификаторов доступа**. При этом пользователь может выполнять только ограниченный набор операций со приватными членами класса, используя публичные методы.
2) Наследование - позволяет определить класс, имеющий определенный набор характеристик (например, методы и переменные экземпляра), а затем создать другие классы, производные от этого класса. Производный класс наследует поля и функции родительского класса и в зависимости от модификаторов доступа самого наследования, полей и методов имеет к ним доступ.
- **Множественное наследование** является особенностью C++, где один класс может наследовать объекты и методы нескольких классов. Конструкторы наследуемых классов вызываются в том же порядке, в котором они перечислены в строке наследования.
3) Полиморфизм - Различное поведение классов наследников, через единый интерфейс базового класса. 
- Статический полиморфизм - на этапе компиляции - CRTP - Шаблоны и Перегрузка функций и операторов.
- Динамический полиморфизм - на этапе выполнения - Виртуальные функции. Точное значение операции определяется объектом для которого она вызывается.

### CRTP
- Curiously Recurring Template Pattern (CRTP) (примерный перевод: Странно рекурсивный шаблон проектирования или Странно повторяющийся паттерн) — идиома языка C++, состоящая в том, что **некоторый класс X наследуется от шаблона класса, использующего X как шаблонный параметр**.
```
Пример:
// The Curiously Recurring Template Pattern (CRTP)
template<class T>
class Base {
  /* methods within Base can use 
  template to access members of Derived */
};
class Derived : public Base<Derived>
{ /* ... */ };
Используется для статического полиморфизма:

template <class T>  struct Base
{
  void interface()
  {
    // ...
    static_cast<T*>(this)->implementation();
    // ...
  }
  static void static_func()
  {
    // ...
    T::static_sub_func();
    // ...
  }
};

struct Derived : Base<Derived>
{
  void implementation();
   static void static_sub_func();
};
```
Источник: https://ru.wikipedia.org/wiki/Curiously_recurring_template_pattern


### Множественное наследование и построение объекта и создание объекта
```
struct Z : AA, AB, AC
{
  MA ma;
  MB mb;
  MC mc;

  Z() {}
}
```
AA AB AC MA MB MC Z ~Z ~MC ~MB ~MA ~AC ~AB ~AA
	
- Создание объекта:
1) БАЗА Слева направо AA AB AC
2) Члены класса сверху вниз МA МB МC
3) Сам класс Z
- Удаление объекта:
1) Сам класс ~Z
2) Члены класса снизу вверх МC МB МA
3) БАЗА справа налево AC AB AA
	
### Виртуальное наследование - зачем нужно:
- Решает проблему ромбовидного наследования
```
class A { int x; };
class B: public A {};
class C: public A {};
class D: public B, public C {};  
D d;
d.x; // проблема, из "x" брать из B или из C
     // в D будет 2 экземпляра "х" 1. B::x и 2. C::x
```
РЕШЕНИЕ 1 - Виртуальное наследование
```
class A { int x; };
class B: virtual public A {};
class C: virtual public A {};
class D: public B, public C {};  
D d;
d.x; // проблемы НЕТ из "x" стало одно A::x
```
- Формально пример ниже, не решает проблему ромбовидного наследования, НО Доступ может быть обеспечен через Двойное двоеточие - "оператор разрешения области видимости" к конкретному х, однако факт того, что их всё равно осталось 2 штуки не изменится
```
class A { int x; };
class B: public A {};
class C: public A {};
class D: public B, public C {};  
D d;
d.B::x;
d.C::x;
```

### PUBLIC PROTECTED PRIVATE наследование
	
http://alenacpp.blogspot.com/2006/03/blog-post_11.html

```
1) public:-> Для производного
public    -> public
protected -> protected
private   -> private

CDerived dd;
//dd.privateBase=3;
//dd.protBase=3;
dd.pubBase=3; //pubBase остался с уровнем доступа public, 
              //к нему теперь можно доступиться.
	
2) protected: -> Для производного
public	  -> protected
protected -> protected
private	  -> private

CDerived dd;
//dd.privateBase=3; недоступно
//dd.protBase=3; недоступно
//dd.pubBase=3; недоступно
	
3) private: -> Для производного
public	  -> private
protected -> private
private	  -> private

При вызове извне, доступиться не получится ни к чему
CDerived dd;
//dd.privateBase=3; недоступно
//dd.protBase=3; недоступно
//dd.pubBase=3; недоступно
```

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
### static_cast и dynamic_cast разница -> 
- dynamic_cast делает проверку на соотвествие типов
- static_cast НЕ делает проверку на соотвествие типов
	
===================================================================
### Виртуальный конструктор

- Его в С++ нет, но существует идиома, с помощью которой можно имитировать его работу.
- Фабричный метод (на базе обобщенного конструктора, паттерны Factory Method и Prototype) - хоть и предназначен для создания объектов без указания их конкретных типов, однако его поведение строго говоря не равно поведению виртуального конструктора, хотя смотря, что под его поведением вообще понимать.
- Подробнее тут http://cpp-reference.ru/articles/virtual-constructor/

===================================================================

### unique_ptr и shared_ptr Коротко
- shared_ptr — для совместного владения, когда на объект может указывать много shared_ptr (можно создавать копии) и объект убивают, только когда все ссылки помрут.
- unique_ptr — уникален, т.е. нескольких unique_ptr, указывающих на один и тот же объект, быть не может (копирование запрещено). Но при этом разрешена передача владения от одного unique_ptr другому, с одновременным уничтожением первого.

### unique_ptr ПОДРОБНО
- НЕТ счётчика ссылок, как в shared_ptr
- Защищён от копирования (помещением в private конструктора копии и оператора присваниявания)
- Нельзя копировать, можно только перемещать при помощи move-семантики
- имеет метод reset() (c++23) - вызывающий деструктор для старого объекта

```
std::unique_ptr<Foo, D> up(new Foo(), D());  // up owns the Foo pointer (deleter D)
std::cout << "Replace owned Foo with a new Foo...\n";
up.reset(new Foo());  // calls deleter for the old one
```

https://habr.com/ru/post/191018/

### shared_ptr ПОДРОБНО

https://habr.com/ru/post/191018/

- В нём есть объект **СЧЕТЧИК ССЫЛОК**, который хранит в себе **ДВА СЧЁТЧИКА** 1. счётчик сильных ссылок (shared_ptr-ов)  2. счётчик слабых ссылок (weak_ptr-ов) 
- ПЕРВЫЙ **АТОМАРНЫЙ счётчик СИЛЬНЫХ ССЫЛОК** (shared_pointer-ов) ссылок, число ссылающихся на объект классов, которые используют указатель на объект
- ВТОРОЙ **АТОМАРНЫЙ счётчик СЛАБЫХ ССЫЛОК** (weak_pointer-ов)
- указатель на сам объект
- и все это аллоцируется внутри так называемого **УПРАВЛЯЮЩЕГО БЛОКА**

### ПОТОКОБЕЗОПАСНОСТЬ shared_ptr
Счётчик ссылок Атомарный, Но сам по себе shared_ptr **НЕ ПОТОКОБЕЗОПАСНЫЙ**, Атомарность счётчика не влияет на потокобезопасность самого объекта на который указывает shared_ptr.

### make_shared()

https://habr.com/ru/post/509004/

В чём особенность и плюсы make_shared() перед shared_ptr()
- Принцип работы: Он передаст параметры в конструктор своего параметра шаблонного some_struct и там же внутри вызовет placement_new, чтобы выделить память под объект на месте по известному адресу памяти.
1) **ОСОБЕННОСТЬ_0** У нас пропадает вызов **new**, который имеет место быть, когда мы используем обычный  std::shared_ptr<int> x(new int);
2) **ОСОБЕННОСТЬ_1** При вызове shared_ptr происходит две алокации 1. Под сам объект и под 2. управляющий блок
- make_shared же при создании выполняет одну аллокацию и при помощи ellidge - смещения, указатель и управляющий блок лежат рядом. ( make_shared + Выделяет память на счётчики ссылок и на объект одним блоком. Меньше проблем с фрагментацией памяти, всё расположено близко, а значит всё более дефрагментированно хранится )
3) **ОСОБЕННОСТЬ_2** Как мы знаем в shared_ptr 2 указателя на сиьные ссылки и на слабые ссылки. 
- ЕСЛИ мы создали shared_ptr и имеем Ноль Сильных ссылок (shared_ptr-s), но есть ещё Слабые ссылки (weak_ptr-s), ТО объект удалится, но управляющий блок останется жить.
- А ЕСЛИ мы создали make_shared наш объект, то даже если счётчик Cильных ссылок стал ноль, а weak_pointer-счетчик ссылок !=0 (weak-ссылки ещё остались), **ТО ОБЪЕКТ НЕ УДАЛИТСЯ**, ИБО у нас была одна общая аллокация.

4) + Нет проблемы с брошенным исключением при вычислении аргументов функции - foo(shared_ptr<A>(new A), bar()). Порядок вызова аргументов и последовательность вызова bar() / new A / shared_ptr не определена строго стандартом, так что Если вызвалось 1. new A 2. вызвалось bar() и bar() вдруг бросит исключение и т.к. конструктор shared_ptr ещё не вызывался, вызвался конструктор только самого объекта А, и память выделенная под А утечёт

- https://youtu.be/wu2FJcfNn3w?list=PLlb7e2G7aSpTFea2FYxp7mFfbZW-xavhL&t=1952
- https://habr.com/ru/post/509004/

process(std::shared_ptr<Bar>(new Bar), foo());

- Плюсы make_shared:
```
 shared_ptr<some_struct> x(new some_struct(a, b, c));
 auto ptr = make_shared<some_struct>(a, b, c);
```	

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
### Сравнение float-чисел
- Для корректного сравнения из-за потери точности, потребуется использовать константу специальную Эпсилон, а именно конструкцию ( abs(a) - abs(b) ) < EPSILON

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

### Хеш таблицы, Может ли хеш таблица быть без коллизий для строк произвольной длинны ?
**ОТВЕТ**: Если множество элементов, которому принадлежат Значения хеш-таблицы БОЛЬШЕ, чем множество из всех возможных Ключей хеш-таблицы, ТО в таком случае в хеш таблица ГАРАНТИРОВАННО будут коллизии. Если множество значений меньше множества ключей, то есть вероятность, что коллизий не будет вообще или их будет минимальное число.

Пояснение:
- Хеш таблица хранит данные в формате [ключ , значение], где ключ = хеш
- В большинстве языков размер ключа это INT [ -((2^31)-1) ; (2^31)-1 ] или UINT [ 0 ; (2^32)-1 ]
- Для диапазона значений типа char (всего 256 различных char-ов мы можем построить хеш таблицу без Коллизий, ИБО ключей (2^32)-1 штук, а значений всего 256)
- Так как в качестве ключа мы указали строки произвольной длинны, а произвольных строк и текстов бесчисленное множество и явно больше чем (2^32)-1 штук, т.е. больше, чем доступных нам ключей, то в этих условиях у нас ГАРАНТИРОВАННО будут коллизии

===================================================================

### Лямбды lambda:
**ВАЖНО** - Нужно следить за жизнью лямда выражения и за временем жизни того, что она использует. Ибо она продолжит работать с объектами, которые уже не существуют, то мы столкнёмся с потенциальными проблемами

В C++ 11 лямбда-выражение - это удобный способ определения объекта анонимной функции (замыкания) непосредственно в расположении, где оно вызывается или передается в качестве аргумента функции

```
- Хороши для 
// Хорошо: Мы можем хранить лямбду в именованной 
// переменной и передавать её в функцию в качестве параметра
auto isEven{   [](int i) { return ((i % 2) == 0); } };
return std::all_of(array.begin(), array.end(), isEven);
```

```
- И примерно такого же аналога 
  в Qt для функции connect 
  -> connect(this, someSig, this, 
	[]{ slot-logic right away })

[ captureClause ] ( параметры ) -> возвращаемыйТип
{ стейтменты; }
Поля captureClause и параметры могут быть 
пустыми, если они не требуются программисту.
Поле возвращаемыйТип является опциональным,
и, если его нет, то будет 
использоваться вывод типа с помощью
ключевого слова auto.
```
- **"Замыкание"**
	
	auto found{ std::find_if(arr.begin(), arr.end(), [search](std::string_view str) {
	
Поле capture clause используется для того, чтобы предоставить (косвенно) лямбде доступ к переменным из окружающей области видимости, 
к которым она обычно не имеет доступ. Всё, что нам нужно для этого сделать, так это перечислить в поле capture clause объекты, 
к которым мы хотим получить доступ внутри лямбды. В нашем примере мы хотим предоставить лямбде доступ к значению переменной search, поэтому добавляем её в захват

- **"Замыкание" / "Захват"** 
1)  Ключевой момент: Переменные, захваченные лямбдой, являются клонами переменных из внешней области видимости, а не фактическими «внешними» переменными.
2) Для продвинутых читателей: Когда компилятор обнаруживает определение лямбды, он создает для нее определение как для пользовательского объекта. 
3) Каждая захваченная переменная становится элементом данных этого объекта. Во время выполнения программы, при обнаружении определения лямбды, создается экземпляр объекта лямбды и в этот момент инициализируются члены лямбды.
- **Захват по значению**
Чтобы разрешить изменения значения переменных, которые были захвачены по значению, мы можем пометить лямбду как mutable. 
В данном контексте, ключевое слово mutable удаляет спецификатор const со всех переменных, захваченных по значению:
- **Захват по ссылке** - Чтобы захватить переменную по ссылке, мы должны добавить знак амперсанда (&) к имени переменной, которую хотим захватить.
    - В отличие от переменных, которые захватываются по значению, переменные, которые захватываются по ссылке, не являются 
    - константными (если только переменная, которую они захватывают, не является изначально const)

```
int  a = 0;
int  b = 0;
auto f = [a, &b](int m, int &n) {
    // a, b внутри [] являются клонами переменных
    int z = 0;
    z++;

    // a++; // **Захват по значению** ERROR read-only variable
    z = z + a; // ok

    b++; // Ok **Захват по ссылке**

    z = z + m;
    n = z;
};
```


 
===================================================================
- Шаблоны с переменным числом аргументов temaplate<typename T, typename ... Args>
- **using** - шаблонный typedef в C++11, он же typedef, который может работать с шаблонами
- ** = default;** - для ЯВНОГО получения реализации конструктора / деструктора и т.д. по умолчанию
- ** = delete;** - попросить компилятор не генерировать один из спец методов констр. копии, оператор присваивания и т.д.
- **override** - мы именно перегрузили метод, который был объявлен как virtual
- **final** - указывается у вирт.метода в конце, чтобы запретить переопределять виртуальный метод в классах наследниках (ВТОРОЕ НЕ ТОЧНО - нужен затем, чтобы никто, кто наследуется от этого класса не мог бы перегрузить его)
- **decltype** - получает выражение и вычисляет его тип. Превратится в тип, который будет по итогу того выражения которое будет при выполнении выражения внтури decltype
```
int bar() 
decltype(std::declval<foo>().bar()) n = d;
перед n подставится int	
```
- **declval** - https://habr.com/ru/company/pvs-studio/blog/592217/
- std::declval<T> – это шаблон функции без реализации, который возвращает rvalue-ссылку на тип T. При типе T = void возвращает тип void. Этот шаблон чаще всего применяется внутри невычисляемого контекста (decltype, sizeof, requires,....) и позволяет как бы работать с объектом переданного типа, обходя вызов конструктора. Это особенно полезно, если тип T не имеет публичного конструктора по умолчанию либо он удален.
- **constexpr** const_expr - для константных выражений, которые могут быть вычислены на этапе компиляции. Если не могут, то constexpr выдаст оишбку
===================================================================

### Размер объекта
Размер объекта в общем случае равен = (1) + (2) + (3) + (4) 

- МЕТОДЫ НЕ ВЛИЯЮТ НА РАЗМЕР ОБЪЕКТА !!! Ответ: 
1) (1) + sizeof всех членов класса 
2) (2) + остаток для выравнивания (по умолчанию выравнивание 4 байта) 
3) (3) + vtblptr = sizeof указателя на vtable (если есть виртуальные функции, 
который находится вскгда ПЕРВЫМ в памяти объетка, чтобы имея указатель на базовый класс 
ты в любом случае мог бы обратиться ко всем функциям (любой из на любой ступни наследования) ) 
4) (4) + указатели на классы предков, от которых было сделано виртуальное наследование 
(=размер указателя * количество классов, есть множ.насл. от двух классов, значит и будет 2 указат. на них) 
(0) - методы внутри объекта не влияют на размер объекта (static) - static-поля НЕ ЯВЛЯЮТСЯ ЧАСТЬЮ КЛАССА

### Пустой объект
Занимает минимум 1 байт, чтобы любой объект можно было отличить друг от друга

===================================================================
### lvalue и rvalue
https://tproger.ru/articles/move-semantics-and-rvalue/
- lvalue - то, что имеет конкретный адрес в памяти
- rvalue - то, что адреса не имеет, то, что называется временным объектом (то, что может стоять справа от знака равенства)
- lvalue в том, что объекты rvalue могут быть перемещены, тогда как объекты lvalue всегда копируются.
- При этом Rvalue может быть Lvalue. Lvalue НЕ может быть RValue
- слева то что имеет адрес, справа что имеет либо не имеет адрес (временный объект)
- Если lvalue ссылку нельзя разрушить
- rvalue ссылка, если строка str = std::move("abc") - перетаскиваем указатель, 

```
void f(int& v); // приоритет ВЫШЕ у этой функции
void f(int&& v);
	
int a = 10;
f(a);  // a формально и rval и lval => вызовется int& для lval, ибо она приоритетнее
f(42); // 42 ravl => вызовется int&& 
f(move(a)) // move привёл тип a явно к rval => явно вызовется int&&
```

```
int i = 1;   // единица это rvalue
int j = i--; // тут i-- это rvalue

int &&ref9{ 5 }; // OK // пятёрка без Адреса
int ref10=0;     // OK
ref10 = ref9;    // OK // всё равно что ref10 = 5, пятёрка без Адреса помещается в ref10
ref9 = 6;        // OK
```

### ОСОБЕННОСТЬ std::move ! https://habr.com/ru/post/587644/

1) (!!!) **std::move - ничего не перемещает, просто делает cast, приведение к r-value ссылке**. ИЛИ функция move - она делает так, что ее аргумент **ПРИТВОРЯЕТСЯ "БЕЗАДРЕСНЫМ"**, т.е. временным.
```
int value = 100;
int moved = std::move(value);
// И value = 100 и moved = 100, произошло ОБЫЧНОЕ Копирование по оператору =
// Причина: 
// int moved = static_cast<int&&>(value);
// int moved = int&&(100); // что эквивалентно int moved = 100;
// После чего выполнится оператор равно и безадресное 100 запишется в moved
```
2) **(!) ПЕРЕМЕЩАЕТ - Перемещающий Оператор и Конструктор Перемещения**

- 1.1. vector<uint8_t> big_vector(1e9, 0);
- 1.2. vector<uint8_t> reciever(big_vector);
- обычное копирование, в big_vector 10^9 нулей и в reciever 10^9 нулей

- 2.1. vector<uint8_t> big_vector(1e9, 0);
- 2.2. vector<uint8_t> reciever(std::move(big_vector));
- big_vector ПУСТ, а в reciever 10^9 нулей, которые перенеслись из big_vector
- время выполнения уменьшилось почти в 10 раз, а размер исходного вектора (big_vector) стал равен нулю.

- 2.2. vector<uint8_t> reciever(std::move(big_vector));
**ОБЪЯСНЕНИЕ**: В Векторе определён перемещающий конструктор в котором определено перемещающее поведение, и именно по тому, что в  конструкторе перемещения описано, что исходный вектор должен обнулиться, а его содержимое переместиться в reciever вектор, именно это и происходит.

НО std::move - ничего не переместил, а сделал ТОЛЬКО cast, приведение к r-value ссылке. Перемещение выполнил именно Оператор перемещения, КОТОРЫЙ вызвался из-за того, что std::move => преобразовал входной параметр к &&, что привело к вызову именно перемещающего конструктора

Аналогично будет и с std::string

===================================================================
### null_ptr
Rengen, для обнуление указателя лучше использовать nullptr, т.к.:
"Причина введения nullptr в язык С++ заключается в устранение возможных неоднозначностей / недоразумений между (макросом NULL = 0) литералом 0 как целочисленным значением и указателем. Двойное значение литерала 0 может вызывать проблемы при некоторых обстоятельствах. Литерал nullptr имеет тип std::nullptr_t, и его нельзя перепутать со значением другого типа. Он может быть неявно преобразован в любой ссылочный тип, но ни в один целочисленный тип, кроме типа bool." - Айвор Хортон, Visual C++ 2010, Глава 4 *примечание.
```
f(int a);
f(int* p);
f(0) - обишка
f(nullptr)
- nullptr приводится к нулевому указателю на любой тип
```

- Для C++ более старых версий nullptr легко добавляется одной строчкой:
- C++ Выделить код 
	static struct nullptr_t {template<typename T> operator T* () const {return 0;}} nullptr;
	
### NULL
Страуструп описывал про макрос NULL:
- 5.1.1. Ноль.
В языке С было очень популярно определять макрос NULL для представления такого нулевого указателя. Так как в C++ типы проверяются более жестко, использование банального нуля вместо NULL приведет к меньшим проблемам. Если вы чувствуете, что просто обязаны определить NULL, воспользуйтесь
const int NULL = 0;

===================================================================
### smart pointers / smart_pointers / умные указатели / умный указатель / типы умных указателей
- https://ravesli.com/urok-195-std-weak_ptr-tsiklicheskaya-zavisimost-s-std-shared_ptr/
- https://eax.me/cpp-smart-pointers/
- https://habr.com/ru/post/140222/

- **auto_ptr** - deprectaed в С++11, убран из C++17, заменён unique_ptr-ром - реализует разрушающее копирование, тоесть имея 2 указателя и при передаче ссылки на объект из одного указателя в другой. Второй куда произошла передача станет "владельцем объекта", а Исходный указатель становится невалидным.
- **unique_ptr** - НЕТ счётчика ссылок, можно move-ать, нельзя копировать - когда ресурс памяти не должен был разделяемым (у него нет конструктора копирования), но он может быть передан другому unique_ptr или владение им с помощью std::move. представляет собой уникальный указатель на объект. Указатель нельзя копировать, но можно передавать владение им с помощью std::move. При уничтожении указателя автоматически вызывается деструктор объекта, на который он указывает.
- **shared_ptr** - СЧЁТЧИК (atomic сильных + atomic слабых) ССЫЛОК  - должен использоваться, когда ресурс памяти должен быть разделяемым. (переменная, которая хранит количество указателей, которые ссылаются на объект, Если эта переменная становится равной нулю, то объект уничтожается. счётчик инкрементируется при каждом вызове либо оператора копирования либо оператора присваивания)
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

### Склейка ссылок / Правило склейки ссылок

- Если при разворачивании шалона, 
получается lval-ref на lval-ref =>
1. T& & => T& 
2. T& && => T& 
3. T&& & => T& 
4. T&& && => T&& 

===================================================================

### STD::MOVE
Ответ: Она преобразует любой входной тип к RValue-ссылке ! (std::remove_reference "Очищает" тип от ссылочности вообще и подставляет после тип RVALUE-ссылку return static_cast ... type&&> )
```
template<typename _Tp>
  inline typename std::remove_reference<_Tp>::type&&
  move(_Tp&& __t)
  { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }
```

### STD::FORWARD

https://ru.stackoverflow.com/questions/908361/%D0%A7%D0%B5%D0%BC-%D0%BE%D1%82%D0%BB%D0%B8%D1%87%D0%B0%D1%8E%D1%82%D1%81%D1%8F-%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D0%B8-move-%D0%B8-forward

Ответ: Она может преобразовать входнорй тип как к LValue-ссылке так и к RValue-ссылке с использованием внутри себя механизм **СКЛЕЙКА ССЫЛОК** 
```
template<class _T>
_T&& forward(typename std::remove_reference<_T>::type& a)
  { return static_cast<_T&&>(a); }
```
Концептуально: Нужна для того, чтобы осуществлять "Проброс типа" того типа колторый нам нужен, через шаблонную функцию так чтобы на выходе этот тип либо в неизменном виде остался. Ибо в результате работы с сылками при работе склейки ссылок мы можем получить не тот тип, на выходе, который мы хотим. Для решения этой проблемы и есть std::forward.

Суть этой идиомы в том, что если ссылка на шаблонизированный тип имеет строго вид T&&, и если в процессе вывода типа компилятор выполняет свертывание ссылок, то в результате свертывания результирующая ссылка может быть выведена как rvalue, так и lvalue.

Нужно для прямая передача **"прямой передачи"** (perfect forwarding), а выглядит эта идиома так:
```
template <typename T>
void f(T&& arg) {
  . . .
  g(std::forward<T>(arg));
}
```

Идеальная передача позволяет создавать функции-обертки, передающие параметры без каких-либо изменений (lvalue передаются как lvalue, а rvalue – как rvalue) 
и тут **std::move** нам не подходит, так как она безусловно приводит свой результат к rvalue.
Поэтому, была разработана функция **std::forward**, которая выполняет примерно следующую работу:
То есть, если ссылка была передана как rvalue, то вызываем std::move, а иначе просто возвращаем то, что передали.

1. Если необходимо «передать» умный указатель **std::unique_ptr**, то сделать мы можем это только через std::move
2. использование std:move можно найти в различных алгоритмах, где нужно менять элементы местами
3. Быстрый Переброс данных из вектора в вектор
```
v2.insert
(
  v2.end(),
  std::make_move_iterator(v1.begin() + 7), 
  std::make_move_iterator(v1.end())
);
As others have pointed out, first vector v1 
is now in indeterminate state, 
so use erase to clear the mess:
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
Как можно создать объект класса по определённому адресу в памяти ? по конкретному адресу в памяти ?
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
### ключевое слово const ?
1) Позволяет задать константность объекта (Чтобы не было возможности изменить ОБЪЕКТ)
2) Позволяет задать константность указателя (Чтобы не было возможности изменить указатель)
3) Константы и аргументы/результаты функций, а так же Позволяет указать, что данный метод не модифицирует члены класса, т.е. сохраняет состояние объекта. Чтобы метод не мог изменять члены класса, ЗА Исключением членов mutable.

- const int i(1);
i = 2; // ERROR;

- const <-- VALUE IS CONST (cip=&v Ok) <-- |*| --> (*ipc=v Ok) PTR IS CONST --> const

```
{
  int a  = 1;
  int b  = 2;
  int aa = 11;

  // [CONST INT <=> INT CONST] POINTER
  const int *cip = &a;
  int const *icp = &b;
  //*cip         = aa;  // compile ERROR // assignment of read-only location ‘cip’
  cip = &k;             // Ok

  int c  = 3;
  int cc = 33;
  
  // [CONST INT <=> INT CONST] POINTER
  int *const ipc = &c;
  *ipc  = cc;  // Ok
  //ipc = &cc; // compile ERROR // assignment of read-only variable ‘ipc’
  
  // ALSO 1
  const char * const s = "data"; // работает
  // ALSO 2
  int i = 4; 
  int* const j(&i); 
}
```

- CONST-метод класса
Помним про const-getter-ы которым может быть нужен mutable mutex для многопоточных операций
```
class A {
private:
  int x;
public:
  void f(int a) const {
    x = a; // <-- не работает
  }
};
```

### Как работает адресная арифметика
```
char* ptr1 = 0x00aaff;
int* ptr2 = 0x11aaff;
ptr1 += 10; // прибавится 10 единиц (произойдёт смещение адреса см_ниже_1 ) ибо char тип размером 1 байт
ptr2 += 10; // 

// см_ниже_1 произойдёт смещение адреса
//                                      0        1        2        3        4        5        6        7        8        9       10
// с 0x00aaff до 0x00ab09 тоесть 0x00aaff 0x00ab00 0x00ab01 0x00ab02 0x00ab03 0x00ab04 0x00ab05 0x00ab06 0x00ab07 0x00ab08 0x00ab09
// см_ниже_2 произойдёт смещение адреса
//                                      0        4        8       12       16       20       24       28       32       36       40
// с 0x00aaff до 0x00ab09 тоесть 0x00aaff 0x00ab03 0x00ab09 0x00ab0c 0x00ab0f 0x00ab13 0x00ab17 0x00ab1b 0x00ab07 0x00ab08 0x00ab09
```

### Через что можно выразить оператор РАВНО и оператор БОЛЬШЕ ?
ОТВЕТ: Операторы «больше» и «равно» выводятся из одного оператора «меньше»
Чтобы поддерживать SORT, FIND и т.д. НЕ нужен оператор equality (a == b)
А НУЖЕН  equivalence  это когда первое значение не меньше второго,
ни второе значение не меньше первого => 
	
**(!(a < b) && !(b < a))**

- оператор БОЛЬШЕ > выражается через меньше => возвращает тру, когда не меньше и не равно => ()
- оператор РАВНО ==  => так же выражается через меньше (не меньше одног и не меньше другого значит РАВНО)
	
### Хотим сложную структуру сделать ключём map/set или unordnered_map/set
		    
- Для **MAP/SET**

https://dawnarc.com/2019/09/c-how-to-use-a-struct-as-key-in-a-std-map/
		    
Переопределение **operator < / operator< / operator меньше** (operator== определяется через operator< а именно "(!(a < b) && !(b < a))" )
```
// Works // Old but Gold
bool compareFunc(const Param& lhs, const Param& rhs)
{
  bool res = false;
  if (lhs.param_1 < rhs.param_1)
  { res = true; }
  else if(lhs.param_1 == rhs.param_1)
  {
    if (lhs.param_2 < rhs.param_2 )
    { res = true; }
    else if(lhs.param_2 == rhs.param_2)
    {  ...
      {
        res = lhs.param_N < rhs.param_N;
      }
    }
  }
  return res;
}
```
- Для **UNORDNERED_MAP/SET**
	
https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
	
1) В структуре/классе ключа - Переопределить **operator== / operator == / operator сравнения** - сравнения ключей должно учитывать, что могут быть хеш-коллизии
2) Дописать **функцию хеширования** (написать класс с переопределённым operator()(const Key& k), который принимает const Key& вызисляет сам хэш из данных Key-структуры и передать написанную нами функцию хеширования 3-тьим параметром в контейнер unordnered_MAP/SET (std::unordered_map<Key,std::string,KeyHasher>)
```
#include <iostream>
#include <unordered_map>
	
struct Key
{
  std::string first;
  std::string second;
  int         third;

  bool operator==(const Key &other) const
  { return (first == other.first
            && second == other.second
            && third == other.third);
  }
};

struct KeyHasher
{
  std::size_t operator()(const Key& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    return ((hash<string>()(k.first)
             ^ (hash<string>()(k.second) << 1)) >> 1)
             ^ (hash<int>()(k.third) << 1);
  }
};

int main()
{
std::unordered_map<Key,std::string,KeyHasher> m6 = {
    { {"John", "Doe", 12}, "example"},
    { {"Mary", "Sue", 21}, "another"}
  };
}
```


### Q: Сложность алгоритма
- Сложность алгоритма - в общем случае, это количественная характеристика, которая говорит о том, сколько времени, либо какой объём памяти потребуется для выполнения алгоритма.
- Измеряется сложность алгоритмов в элементарных шагах - то, сколько действий необходимо совершить для его выполнения. Любой алгоритм включает в себя определённое количество шагов и не важно на каком устройстве он будет запущен, количество шагов останется неизменным. Эту идею принято представлять в виде Big O (или О-нотации).
- Big O показывает то, как сложность алгоритма растёт с увеличением входных данных. При этом она всегда показывает худший вариант развития событий - верхнюю границу.

### Q: Двоичный поиск / Бинарный поиск / сложность бинарного поиска
- Особенность: Бинарный поиск можно осуществлять ТОЛЬКО внутри отсортированного массива
- Сложность O(log(n))

### Разница между оператором [] и оператором at() в векторе vector
- vector.at(i); Делает попытку обратиться к элементу и  Бросает исключение std::out_of_range  при выходе за пределы вектора 
- operator[] делает попытку считать данные и падает без бросания исключения

О векторе:
- удаление из начала из вектора - O(n), ибо он пересоберётся
- добавление/удаление в/из кононец - АМОРТИЗИРОВАННОЕ O(1)
- Хочу удалить элемент в начале или всередине за константное время и мне не важен порядок. ОТВЕТ: Делаем swap(удаляемого,последнего) и удаление последнего.
- std::sort() - используется quick-sort, поэтому его можно применить к std::vector, но нельзя применить к std::list, ибо в list у нас нету random-acces-operator. поэтому в std::list::sort() - есть свой собственный метод для сортивровки.

### ITERATOR - ВАЛИДНЫЕ / НЕВАЛИДНЫЕ ИТЕРАТОРЫ

- list - НЕ Инвалидирует ИТЕРАТОРЫ - указатели переуказывают на другие элементы просто и всё хорошо с итераторами
- дерево - НЕ Инвалидирует ИТЕРАТОРЫ - указатели переуказывают на другие элементы просто и всё хорошо с итераторами ибо это структура с указателями на left и на right и поэтому всё ровно. Самобалансирующиеся деревья AVL () / Красно-чёрное / Декартовы рандомизированные деревья, у всех разные условия балансировки, в зависимости от версии компилятора.
- хэш-таблица - ИНВАЛИДирует ИТЕРАТОРЫ - использует под собой динамический массив. И когда есть коллизии образуются связные списки, дойдя до каких-то пределов хэш-таблица выполнит операцию rehash() переиндексация всей хеш-таблицы, и это инвалидирует итераторы (1. Метод цепочек - перехеширование происходит при наличии (4n/3) элементов 2. Открытая Адресация - перехеширование надо проводить при неполном заполнении хеш-таблицы.)
- vector - ИНВАЛИДирует ИТЕРАТОРЫ - В векторе происходит реаллокация памяти, как только size уходит за capacity
- std::priority_queue<int> pq; - ИНВАЛИДирует ИТЕРАТОРЫ - // основан на куче HEAP - вставка и удаление O(log(n)) ибо приходится выполнять операцию hepiphy, т.е. просеивать кучу из-за чего итераторы в результате становятся невалидными после просеивания
- std::stack и std::queue - основаны на std::deque
- std::deque - наборы массивов кусочками связанные и существующие указатель конца одного массива указывает на начало следующего и т.д. хранится блоками
- std::deque - дек, двусторонняя очередь - вствка и удаление в конец и в начало О(1), в остальные места О(n)
	
### ALLOCATOR Алокатор Аллокатор - Хотим хранить Node-ы в std::list так, чтобы элементы стяли рядом
- Для этого надо написать свой класс/структуру АЛЛОКАТОР, и передать его последним параметром в шаблоне листа при создании std::list<int, allocator> _list;

### Типы Аллокаторов Allocator types
- malloc - General purpose allocator - стандартный аллокатор, который попытается выделить потенциально любое количество памяти в рамках доступной. (не следит за сегментированием памяти. не гарантирует, что наши данные будут расположены последовательно (Из-за чего могут быть потенциальные промахи в кэшэ процессора). потокобезопасен в случае использования библиотеки pthread) (при выделении использует sbrk или в случае большиъ объёмов более чем MMAP_TRESHOLD байт, использует mmap)
- Почему используют именно malloc по умолчанию, а не Кастомные (см. ниже) - в большинстве проектах обычно нет жёствких критериев по производительности системы касательно выделения и управления памятью, и работа по доработке собственного аллокатора может быть банально избыточной (доп время и сложности при разработке ПО).

Чтобы избежать проблем malloc-а связанных с производительностью нам и нужны Собственные / Кастомные / Альтернативные аллокаторы:
- LinearAllocator
- PoolAllocator
- StackAllocator
- ArenaAllocator

# Типы контейнеров:

- bitset	 Битовое множество	<bitset>
- array		 Статический массив	<array>
- vector	 Динамический массив	<vector>
- forward_list	 Линейный односвязный список	<forward_list>
- list		 Линейный двусвязный список	<list>
- deque		 Дек (двусторонняя очередь, очередь с двусторонним доступом)	<deque>
- set		 Множество, в котором каждый элемент уникален	<set>
- map		 Отображение. Хранит пары "ключ-значение", в которых каждый ключ связан только с одним значением	<map>
- multimap	 Мультиотображение. Хранит пары "ключ-значение", в которых каждый ключ может быть связан с двумя или более значениями	<map>
- multiset	 Множество, в котором каждый элемент необязательно уникален	<set>
- priority_queue Приоритетная очередь	<queue>
- queue		 Очередь	<queue>
- stack		 Стек	<stack>



При любой операции в середине вектора выделение памяти ?
УДАЛЕНИЕ ...
При расширении Капасити только при добавлении в вектор
При удалении элемента вектор не перераспределяется не перекопируется полностью
Вектор 2^n зависит от версии компилятора.
	
# Array (STL-ная обёртка обычного **статического массива на СТЕКЕ**)
- Array - фиксированный размер, vector - динамический. 
- std::vector динамический массив, хранящийся В КУЧЕ
- std::array статический массив, хранящийся НА СТЕКЕ 
- std::vector - это шаблонный класс, который инкапсулирует динамический массив, хранящийся в куче. Механика сжатия/расширения автоматизирована (т.е. производится при удалении/добавлении элементов).
- std::array - же в свою очередь инкапсулирует статический массив, который хранится собственно внутри объекта. Таким образом, если вы создадите объект на стеке, то и массив будет на стеке. Как следствие, его размер должен быть известен на этапе компиляции и, естественно, его нельзя сжать/расширить. 
	
# Vector
```
 begin()       last() end()
   V	          V    V
 | 1 | 2 | ... | 10 | sz+1 |  |  | ... |  | 
 \\__________v______/                     /
  \________size()__________v_____________/
	                capacity() = (2*size)
	
```
- В основе - **динамический массив (В КУЧЕ)**, занимает смежный или непрерывный участок в памяти, с О(1) доступ к произвольному элементу по индексу
- Метод .data() - Получение указателя этот самый смежный кусок памяти. Указатель этот валиден до тех пор пока мы не РЕАЛЛОЦИРУЕМ / перекопируем весь вектор в случае его расширения
- При любой операции вставки (в любое место) push_back / insert может случится реаллокация с расширением его capacity в 2 раза и с полным перекопированием всего вектора, в новую реаллоцированную память.
- Вставка в начало O(n) вызовет "сдвиг вправо на 1 эелемент всего вектора в рамках capacity", то есть копирование всех элементов вектора не одну позицию вправо, чтобы потом затереть старый первый элемент и поместить на его место новый первый, что мы хотели вставить. insert(v=9,pos=0) 1. v={2,3,4} 2. v{2,2,3,4} 3. v={9,2,3,4}
- Вставка в середину O(n) вызовет сдвиг вправо относительно места вставки всех элементов, и размещение нового элемента на месте строго. insert(v=9,pos=1) 1. v={2,3,4} 2. v{2,3,3,4} 3. v={2,9,3,4}
- Удаление из начала вызовет сдвиг и копирование всех элементов за О(n), которые были справа, на одну позицию влево и delete для последнего.
- Удаление из середины вызовет частичный сдвиг и копирование всех элементов за О(n), которые были справа от удаляемого, на одну позицию влево и delete для последнего.
- Вставка в КОНЕЦ вектора занимет - **АМОРТИЗИРОВАННОЕ** O(1), ибо пока вектор не нужно расширять (size <= capacity), то новый элемент за О(1) помещается в конец вектора, как только надо вставить элемент в конец, когда capacity уже не хватает, вектор РЕАЛЛОЦИРУЕТСЯ (выделяет себе памяти в ДВА раза больше старого capacity, и копирует все значения из старого места в новое реаллоцированное (вызываются конструкторы копии у элементов)), что может привести к оценке скорости в О(n), поэтому вектору стоит делать **RESERVE()** если есть понимание на сколько большой он нужен заранее.
- При РЕАЛЛОКАЦИИ элементы копируются, ЕСЛИ хотим, чтобы элементы при реаллокации перемещались std::move-ались, а не копировались, нужно у элементов 1. Конструктор перемещения и 2. Оператор перемещения пометить как noexcept
- Быстрая за Аморт.О(1) Вставка и удаление возможно только с конца вектора.
- Вставка, удаление при использовании insert, erase занимает линейное время О(n), пропорциональное длине последовательности, хранящейся в контейнере.
- Доступ к элементам произвольный. Занимает постоянное время О(1).
- Сортировка возможна с применением внутренней функции sort.
- Вставка в конец требует постоянного времени, но если требуется перераспределение, то требуется линейное время.
- Из всех функций получения элемента вектора наиболее безопасной считается at()  т.к. в случае неверного индекса она выбрасывает исключение out_of_range . Все остальные ([], front, back) обладают неопределённым поведением.
- Итератор остается действительным до момента вставки или удаления элемента с меньшим индексом или перераспределения памяти с изменением ёмкости.
- Вставка и удаление элементов выполняется быстрее, если группа элементов обрабатывается одним вызовом вместо нескольких последовательных вызовов.
- Для любого действительного индекса i в векторе v заведомо истинно следующее условие: &v[i] == &v[0] + i;
- Если используемые элементы не генерируют исключений (в операторе присваивания, копирующем конструкторе, конструкторе), то любая операция  либо выполняется успешно, либо не вносит изменений в контейнер.
- Vector<bool> является особым типом вектора, он не удовлетворяет всем требованиям других векторов. Его итератор не является итератором произвольного доступа. Один элемент вектора занимает один бит памяти.

# Dequeue (двусторонняя очередь)
- В основе - **"список фиксированных векторов" - динамический массив (В КУЧЕ)**, лежит кусками по несколько элементов, каждый кусок это условный "вектор" с начальным элементом и с конечным элементом, в которых находятся указатели на начало и конец другого куска. Быстрая за О(1) вставка в начало и в конец.
- Вставка и удаление возможна в конец .push_back() и начало .pop_back(). Вставка, удаление при использовании insert, erase занимает линейное время, пропорциональное длине последовательности, хранящейся в контейнере.
- Доступ к элементам произвольный. Занимает постоянное время O(1).
- Внутренняя структура содержит ряд ссылок связывающих между собой блоки выделенной памяти, поэтому обращение к элементам и перемещение итератора выполняется медленнее, чем в векторах.
- Деки не позволяют управлять ёмкостью и моментом перераспределения памяти. При вставке элемента в конец, середину или начало, становятся недействительными все итераторы, ссылающиеся на элементы дека. Однако перераспределение памяти выполняется быстрее, чем в векторах из-за особенностей реализации (блочной).
- Вставка или удаление может привести к перераспределению памяти, в результате чего могут стать недействительными все указатели, ссылки и итераторы, ссылающиеся на другие элементы дека. Сюда не относится вставка, удаление с конца, начала дека – в этом случае ссылки и указатели на элементы (но не итераторы) остаются действительными.
- Сортировка возможна с применением внутренней функции dequeue::sort.

# List
- В основе - **динамический массив (В КУЧЕ)**, но каждый элемент в произвольных местах в памяти, с двумя (list двусвязный) или одним (forward_list односвязный) указателями на предыдущий+следующий элементы или только на следующий элемент. (ПЛЮС forward_list, занимает меньше места в памяти)
- Вставка и удаление возможна в произвольном месте, выполняется за постоянное время О(1).
- Нет произвольного доступа, доступ за О(n) только по итератору.
- В результате вставки и удаления элементов указатели, ссылки и итераторы, относящиеся к другим элементам, остаются действительными.
- Список реализован таким образом, что любая операция над ним либо совершается, либо нет. Список не может находиться в промежуточном состоянии.
- Возможна сортировка функцией членом sort.
- Функция-член list<T>::remove – это единственная функция, которая действительно удаляет элементы (выполняет фактическое удаление, т.е. erase вызывать после нее не требуется). Другая же одноименная функция-алгоритм remove занимается уплотнением элементов коллекции, после работы, которой требуется вызывать erase для удаления элементов и изменения размера контейнера.

# Set
- В основе - **Красно-Чёрное дерево** => вставка/удаление/поиск/все операции О(log(n))
- Не содержат дубликатов, элементы автоматически сортируются, из-за чего накладываются ограничения – значения элементов нельзя изменять напрямую, так как это нарушит порядок сортировки.
- Не поддерживают прямое обращение к элементам.
- Элемента двух множеств будут равны только в том случае, если будут равны их критерии сортировки.
- Контейнеры map и set не допускают повторяющихся значений ключей (значение перезапишется)
- set будем использовать, когда мы хотим чтобы элементы были бы по итогу отсортированы
	
# Map (отображения)
- В основе - **Красно-Чёрное дерево** (самобалансирующееся)  => вставка/удаление/поиск/все операции О(log(n))
- Элементы представляются в виде пары – ключ/значение.
- Если захотим использовать в качестве ключа свой самописный класс, то нужно будет определить в этом классе **operator<**   
- Сортировка элементов производится автоматически на основании критерия сортировки, применяемого к ключу. Отличие в том, что мультимапы могут содержать дубликаты.
- Ключ элемента нельзя удалять напрямую, так как это нарушает порядок сортировки. Для этого нужно удалить старый ключ и вставить новый.
- Модификация значения разрешена, если только значение не константное.
- Отображения поддерживают операцию индексирования [key] для прямого обращения к элементам. В случае если элемент с заданным ключом отсутствует в отображении, он автоматически создается и инициализируется значением по умолчанию, базовым конструктором. Таким образом, с помощью оператора индексирования можно добавлять элементы в отображение, однако эффективнее воспользоваться операцией вставки (insert).
- Контейнеры map и set не допускают повторяющихся значений ключей (значение перезапишется)
															  
# Multiset, Multimap (мультиотображения)
- В основе - **Красно-Чёрное дерево** (самобалансирующееся)  => вставка/удаление/поиск/все операции О(log(n))
- Контейнеры map и set не допускают повторяющихся значений ключей, а multimap (мультиотображение) и multiset (мультимножество) позволяют сохранять **ключи с дублирующимися значениями**.

# unordered_set, unordered_map (хеш-контейнер)
- В основе - **хеш таблица**
- Операция вставки - **АМОРТИЗИРОВАННОЕ** O(1), ибо
- 1) ЕСЛИ потребуется переиндексировать всю хеш-таблицу, то выполняется вызов операции **REHASH()** и время вставки из-за этого будет линейным O(n)
- 2) ЕСЛИ будут **коллизии** (разные входные данные, для которых хэш-функция выдала одинаковый хеш), и таких случаев много, то на месте коллизии начинают формироваться **БАКЕТЫ**. Формруются они как правило в виде двусвязных списков (по (1) методу Цепочек, на месте коллизии к концу элемента прикрепляется указатель на следующий элемент, и формируется список из колизионых элементов), что приводит к падению скорости вставки и других операций до уровня и скорости работы как у списка, то есть к О(n). 
- Есть ещё Методы разрешения хеш-коллизий, а именно: 
- (2) Метод Сквозной Адресации, каждый следующий колизионный элемент пытаестя встать на ближайшее "пустое" место ближайшее к элементу, на которое он собирался встать, встаёт просто следующее по прядку место в таблице, 
- (3) Метод Double Hashing - Двойное хеширование - метод для разрешения коллизий - https://www.geeksforgeeks.org/double-hashing/
- (4) Метод Cuckoo hashing - Кукушкино хеширование - https://ru.wikipedia.org/wiki/%D0%9A%D1%83%D0%BA%D1%83%D1%88%D0%BA%D0%B8%D0%BD%D0%BE_%D1%85%D0%B5%D1%88%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5
- Если захотим использовать в качестве ключа свой самописный класс, то нужно будет определить:
- 1) Написать/определить **функцию хеширования** (класс с переопределённым operator()(const Key& k)) и передать 3-им параметром в класс хэш-таблицы 
- 2) **operator== / operator == / operator сравнения** - сравнения ключей должно учитывать, что могут быть хеш-коллизии  
- Операция поиска выполняется за постоянное время O(1).
- Данный контейнер разумно использовать при частых операциях вставки, удаления и поиска.


# Типы итераторов:

- Произвольного доступа (random access)	Сохраняют и считывают значения; позволяют организовать произвольный доступ к элементам контейнера
- Двунаправленные (bidirectional)	Сохраняют и считывают значения; обеспечивают инкрементно-декрементное перемещение
- Однонаправленные (forward)	Сохраняют и считывают значения; обеспечивают только инкрементное перемещение
- Входные (input)	Считывают, но не записывают значения; обеспечивают только инкрементное перемещение
- Выходные (output)	Записывают, но не считывают значения; обеспечивают только инкрементное перемещение

# Проблема увеличения и уменьшения итераторов в векторах 
– При применении к итераторам векторов операций инкремента и декремента – и ++ могут возникать ошибки компиляции. Это обусловлено тем, что итераторы в векторах зачастую реализуются как обыкновенные указатели, а для них, как для всех базовых типов запрещена модификация временных объектов. Данный вызов будет считаться ошибочным sort(++vec.begin(), vec.end()) т.к. begin() вернёт временный объект базового типа.

# Алгоритмы НЕ могут использоваться со СПИСКАМИ, потому что списки не поддерживают итераторы ПРОИЗВ. доступа.

![](Screenshot_1368.png)
	
| Операция                     | Значение                   |
|------------------------------|----------------------------|
| vector +  вставка в Конец:   | O(1) АМОРТИЗИРОВАННОЕ O(n) |
| vector +  вставка в НЕ_Конец | O(n)                       |
| vector -  удален. Конец:     | O(1)                       |
| vector -  удален. Произв:    | O(n)                       |
| vector [] произв. Доступ:    | O(1)                       |
| vector s  поиск НЕсортриров: | O(n)                       |
| vector ss поиск по SORTиров: | O(log(n)) Бинарным поиском |
| vector it валидность итер.:  | нет                        |
	
- АМОРТИЗИРОВАННОЕ ибо, когда size > capacity, то при вставке происходит расширение capacity в 2 раза и происходит полное перекопирование старого вектора в новое место

- Двусторонняя очередь - список из фиксированных векторов

| Операция                            | Значение                   |
|-------------------------------------|----------------------------|
| deque +  вставка в Конец:           | O(1)                       |
| deque +  вставка в Начало:          | O(1)                       |
| deque +  вставка НЕ_Конец_Не_Начало | O(n)                       |
| deque -  удален. Конец:             | O(1)                       |
| deque -  удален. Начало:            | O(1)                       |
| deque -  удален. НЕ_Конец_Не_Начало | O(n)                       |
| deque [] произв. Доступ:            | O(1)                       |
| deque s  поиск НЕсортриров:         | O(n)                       |
| deque ss поиск по сортиров:         | O(log(n)) Бинарным поиском |
| deque it валидность итер.:          | Указатели только           |

- list / forward_list / stack / queue
	
| list and forward_list                    | Значение          |
|------------------------------------------|-------------------|
| list +/-  вставка/удаление в Конец:      | O(1)              |
| list +/-  вставка/удаление в Начало:     | O(1)              |
| list +/-  вставка/удаление по Итератору: | O(1)              |
| list +/-  вставка/удаление по Индексу:   | O(n)              |
| list - произв. Доступ:                   | O(n) (*)          |
| list - поиск:                            | O(n)              |
| list - валидность итер.:                 | ДА                |
	
(*) O(n) - ибо идём по всем элементам через их указатели по всему листу пока не дойдём до указанного индекса
	
- Красно-чёрное дерево
	
| Операция                          | Значение                   |
|-----------------------------------|----------------------------|
| set / map - вставка               | O(log n)                   |
| set / map - удален                | O(log n)                   |
| set / map - произв. Доступ        | НЕТ                        |
| set / map - поиск                 | O(log n)                   |
| set / map - валидность итер.:     | ДА                         |

- хеш-таблица
	
| Операция                          | Значение                   |
|-----------------------------------|----------------------------|
| unordered_set - вставка           | O(1) АМОРТИЗИРОВАННОЕ O(n) |
| unordered_set - удален            | O(1) АМОРТИЗИРОВАННОЕ O(n) |
| unordered_set - произв. Доступ    | O(1) АМОРТИЗИРОВАННОЕ O(n) |
| unordered_set - поиск             | O(1) АМОРТИЗИРОВАННОЕ O(n) |
| unordered_set - валидность итер.: | Указатели только           |
	
- АМОРТИЗИРОВАННОЕ, ибо ПЕРВОЕ(1): на месте КОЛЛИЗИЙ начинают формироваться Бакеты, а бакет в стандартной библиотеке представляет из себя как правило обычный список, а операции в списке в худшем случае О(n)
- АМОРТИЗИРОВАННОЕ, ибо ВТОРОЕ(2): при разростании таблицы, на каком-то этапе вставки нового элемента может случиться REHASH - переиндексация всей хеш-таблицы. Суть: Пока таблица мала, использовалась одна хэш функция, при переполнении таблицы. Для избежания переполнения выбирается новая хеш-функции и (или) хеш-таблица большего размера, и этот процесс называется перехеширование (rehashing).

- heap = priority_queue
	
| Операция                          | Значение                   |
|-----------------------------------|----------------------------|
| priority_queue - вставка          | O(log n)                   |
| priority_queue - удален           | O(log n)                   |
| priority_queue - произв. Доступ   | O(1)                       |
| priority_queue - поиск            | НЕТ - (O(n))               |
| priority_queue - валидность итер. | НЕТ - хз                   |
	
- просеивание кучи

https://flexiple.com/algorithms/big-o-notation-cheat-sheet/
	
- Search Algorithms Алгоритмы поиска

| Search Algorithms	       | Space Complexity	| Time Complexity | -            | -           |
|------------------------------|------------------------|-----------------|--------------|-------------|
|  -                           |     -                  | Best Case	  | Average Case | Worst Case  |
| Linear Search	Линейный поиск | O(1)	                | O(1)	          | O(n)	 | O(n)        |
| Binary Search	Бинарный поиск | O(1)	                | O(1)	          | O(log n)	 | O(log n)    |

- Sorting Algorithms Алгоритмы сортировки

| Sorting Algorithms	    | Space Complexity	  | Time Complexity | -            | -          |
|---------------------------|---------------------|-----------------|--------------|------------|
|                           |                     | Best Case	    | Average Case | Worst Case |
| Selection Sort Выбором    | O(1)	          | O(n^2)          | O(n^2)       | O(n^2)     |
| Insertion Sort Вставками  | O(1)	          | O(n)            | O(n^2)       | O(n^2)     |
| Bubble Sort Пузырьковая   | O(1)	          | O(n)            | O(n^2)       | O(n^2)     |
| Quick Sort Быстрая сорт   | O(log n)            | O(log n)        | O(n log n)   | O(n log n) |
| Merge Sort сорт слиянием  | O(n)	          | O(n)            | O(n log n)   | O(n log n) |
| Heap Sort Пирамидальная   | O(1)	          | O(1)            | O(n log n)   | O(n log n) |



# Сортировки:
http://www.cyberforum.ru/cpp-beginners/thread27084.html

- 1. Сортировка выбором / Selection sort

Идея метода состоит в том, чтобы создавать отсортированную последовательность путем присоединения к ней одного элемента за другим в правильном порядке. Если входная последовательность почти упорядочена, то сравнений будет столько же, значит алгоритм ведет себя неестественно.

- 2. Сортировка пузырьком (обменом) / Bubble Sort

Идея метода: шаг сортировки состоит в проходе снизу вверх по массиву. По пути просматриваются пары соседних элементов. Если элементы некоторой пары находятся в неправильном порядке, то меняем их местами.

- 3. Сортировка вставками / Insertion sort

Аналогично сортировке выбором, среднее, а также худшее число сравнений и пересылок оцениваются как O(n^2), дополнительная память при этом не используется.
Хорошим показателем сортировки является весьма естественное поведение: почти отсортированный массив будет досортирован очень быстро. Это, вкупе с устойчивостью алгоритма, делает метод хорошим выбором в соответствующих ситуациях.

- 4. Сортировка Шелла

Сортировка Шелла является довольно интересной модификацией алгоритма сортировки простыми вставками.
Часто вместо вычисления последовательности во время каждого запуска процедуры, ее значения рассчитывают заранее и записывают в таблицу, которой пользуются, выбирая начальное приращение по тому же правилу: начинаем с inc[s-1], если 3*inc[s] > size.

- 5. Пирамидальная сортировка / Heapsort

Пирамидальная сортировка является первым из рассматриваемых методов, быстродействие которых оценивается как O(n log n).
Построение пирамиды занимает O(n log n) операций, причем более точная оценка дает даже O(n) за счет того, что реальное время выполнения downheap зависит от высоты уже созданной части пирамиды.
Вторая фаза занимает O(n log n) времени: O(n) раз берется максимум и происходит просеивание бывшего последнего элемента. Плюсом является стабильность метода: среднее число пересылок (n log n)/2, и отклонения от этого значения сравнительно малы.
Метод не является устойчивым: по ходу работы массив так "перетряхивается", что исходный порядок элементов может измениться случайным образом.

- 6. Быстрая сортировка (сортировка Хоара) / Quick sort

"Быстрая сортировка", хоть и была разработана более 40 лет назад, является наиболее широко применяемым и одним их самых эффективных алгоритмов.
Каждое разделение требует, очевидно, O(n) операций. Количество шагов деления(глубина рекурсии) составляет приблизительно log n, если массив делится на более-менее равные части. Таким образом, общее быстродействие: O(n log n), что и имеет место на практике.
Итеративный алгоритм быстрой сортировки.

- 7. Поразрядная сортировка / Radix sort

Рассматриваемый ниже алгоритм существенно отличается от описанных ранее.
Во-первых, он совсем не использует сравнений сортируемых элементов.
Во-вторых, ключ, по которому происходит сортировка, необходимо разделить на части, разряды ключа. Например, слово можно разделить по буквам, число - по цифрам..


# Сортировки время работы в ЛУЧшем / СРЕДнем / ХУДшем случае:
https://habrahabr.ru/post/188010/

- 1) Сортировка выбором / Selection sort

ВСЕ СЛУЧАИ: O(n^2)

- 2. Сортировка пузырьком (обменом) / Bubble Sort

Луч: O(n)	Сред: O(n^2)	Худ: O(n^2)

- 3. Сортировка вставками / Insertion sort

Луч: O(n)	Сред: O(n^2)	Худ: O(n^2)

- 4. Сортировка Шелла 

Луч: До O(n)	Сред: плавает*	Худ: O(n^2)    (*) плавает в зависимости от вида входной последовательности.

- 5.1 Пирамидальная сортировка / Heapsort

ВСЕ СЛУЧАИ: O(n*log(n))

- 5.2 Сортировка деревом / Tree sort

ВСЕ СЛУЧАИ: O(n*log(n))

- 6. Быстрая сортировка / Quick sort

Луч: O(n*log(n))  Сред: O(n*log(n))  Худ: O(n^2)

- 7. Поразрядная сортировка / Radix sort ( http://algolist.manual.ru/sort/radix_sort.php )

Луч: O(n)       ВСЕ СЛУЧАИ: O(n*k), n - разрядность данных: количество возможных значений разряда ключа; k - количество разрядов в самом длинном ключе


# Сортировки / Гарантированно работающая за O(n(log(n))) это Пирамидальная сортировка (HeapSort)
https://habrahabr.ru/post/335920/

Пирамидальная сортировка / Heapsort

Развитие идеи сортировки выбором. Воспользуемся структурой данных «куча» (или «пирамида», откуда и название алгоритма). Она позволяет получать минимум за O(1), добавляя элементы и извлекая минимум за O(logn). 

Таким образом, асимптотика O(n(log(n))) в ХУДШЕМ, СРЕДНЕМ и ЛУЧШЕМ случае. Реализовывал кучу я сам, хотя в С++ и есть контейнер priority_queue, поскольку этот контейнер довольно медленный.


	
	
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


### Конструктор копирования / Конструктор копии / КОНСТРУКТОР КОПИЙ

Конутркутор копии вызывается в ТРЁХ случаях:
1) Инициализация класса : ``` MyClass mc = mcz;```
2) Во время ВОЗВРАТА значения из функции :  ``` MyClass doSmth(...); /* code; */ MyClass mc = doSmth(); ```
3) При передаче в качестве АРГУМЕНТА в функцию : ``` void doSmth(MyClass _my); /* code; */ doSomething(mc); ```

Сигнатура конструктора копий:
```
class A {
  public:
    A(const A& that);
};
```

### Виртуальный декстуктор
- Если наш класс будет полиморфным, предполагает быть базовым, иметь наследников, и если мы будем использовать свойства полиморфизма, то есть создавать объекты классов наследников, через указатель на базовый класс, ТО нужно деструктор сделать виртуальным. Ибо пока он не виртуальный он не попадёт в таблицу виртуальных функций производного класса, и при удалении объекта, не будет работать (полиморфизм = механизм виртуальных функций) и мы получим вызов деструктора только базового класса, и деструктор наследника вызван не будет, что приведёт к утечек памяти.

### ЧИСТАЯ Виртуальная функция (метод) 
- метод такого втда: 'virtual method() = 0;' не имеющий реализации в телле класса, а имеющий только Сигнатуру.
- для чистой виртуальной функции можно предоставлять базовую реализацию, обращаться к которой может класс наследник Base_class::pure_virtual_func();
	
### Абстрактный класс
- класс имеющий хотя бы одну чисто виртуальную функцию, без её реализации в потомке
- Экземпляры Абстрактного класса создавать НЕЛЬЗЯ !

```
  Base()
   |
  \ /
 Derived()
------------
~Derived
   |
  \ /
 ~Base

in memory:
    --------------
  0 | Base class |
 10 | ...        |
  n | Derived    |
    --------------
```
При создании объекта конструкторы вызываются
в порядке Сверху-Вниз, т.е. от Базового к Наследнику.
А Декструкторы вызываются в обратном порядке, от наследника к Базовому.

### ВИРТУАЛЬНЫЙ ДЕСТРУКТОР
- если он не указан вызываются лишь те дееструкторы, которые можно определить из иерархии наследования из типа указателя, к которому
	применяется оператор delete

### Функтор это функция с состоянием
Функтор в С++ это объект у которого перегружен operator() т.е. внешне эксземпляр класса может быть похож на функцию и использоваться как функция
	
### Чем отличается копирование от присваивания?
- Копирование - инициализация объектом того же типа. Создаём новый объект "получатель" того же типа (пока пустой) и из объекта "источника" во вновь созданный копируем "содержимое источника".
- Присваивание - замена уже существующего объекта. Не создаём ничего объет "получатель" уже присуствует, и из объекта "источника" в ужеимеющийся копируем "содержимое источника".


### Какие методы в классе определяются неявно

1) Деструктор класса
2) Конструктор по умолчанию (без параметров)
3) Конструктор КОПИРОВАНИЯ. Конструктор копии (1. myclass х = у; // инициализация 2. func (х); // передача параметра 3. у = func (); // возврат из фции и как следствие получение временного
4) Оператор присваивания =
5) С++11: Перемещающий конструктор копии (он же Конструктор перемещения)
6) C++11: Перемещающий оператор присваивания =

```
(!) ПОДРОБНО:

Итак, если вы написали
class A {}; // Empty

То, знайте, что на самом деле вы создали примерно вот такой класс:
class A 
{
public:
  ~A(); // Деструктор
  A();  // Конструктор без параметров
  A(const A & that);        // Копирующий конструктор по умолчанию (тупо копирует все поля)
  A& operator=(const A & that); // Оператор присваивания
  A(A&& that) {} // C++11   // Коснтруктор перемещения (Перемещающий конструктор) 
  A& operator=(A&& that) {} // C++11 // Оператор перемещения (Перемещающий оператор присваивания)
};

//  помним так же про
  Empty * operator&();  // Оператор получения адреса
  const Empty * operator&() const;  // Оператор получения адреса константного объекта

```	

### Какова сигнатура конструктора копии / копирующего конструктора ?
	A(const A& _obj) { this = _obj; }

### Какова сигнатура оператора присваивания / оператора "равно" ?
```
A& operator=(const A& _obj) 
{
  if (&_obj == this)
    return *this;
	
  A a; a = _obj; return a;
} 
```
### Какова сигнатура перемещающего конструктора / конструктора перемещения ?
	A(A&& tmp) { ... } // см 2 вопр. ниже

### Какова сигнатура перемещающего оператора / оператора перемещения ?
	A& operator=(A&& tmp) { ... } // см 1 вопр. ниже

### Размер size_t / Какой тип у size_t
size_t = unsigned int 


### remove_if() с функцией предикатом
```
auto res = remove_if(people.begin(), people.end(), [](const Person &p)
{ return p.Score < 150; });
people.erase(res, people.end());	
```	
	
### Идиома erase-remove Идиома / erase remove идиома нужна для Вектора, а для Списка НЕТ
``` v[7]={1,2,3,2,8,2,7}; ```		       
- auto it = std::remove(v.begin(), v.end(), 2);
- 1. Выполнит смещение всех элементов != 2 влево, в нашем случае 4 элемента будут на первых 4-ёх позициях т.е. v[7]={1,3,8,7,8,2,7}; 
- 2. Длинна вектора при этом не изменится, и remove вернёт итератор it на следующий после 4-го элемента, т.е. it-> index=[5] -> element_value = {8}
- 3. Все элементы после 4-го будут в несогласованном состоянии, скорее всего просто останется то, что было в оригинальном векторе, без изменений. Хотя говорят обычно, что все "не нужные" элементы уберутся в конец вектора, но это происходит не всегда, и конец может и не тронутся вовсе.
- v = erase(it, v.end());
- Просто отрезаем от вектора, начиная с того, на что указывает it, и заканчивая концом вектора, и получаем  v[4]={1,3,8,7}

	
### Удалить все цифры 8 внутри числового вектора:
```
#include <algorithm>

vec.erase( std::remove( vec.begin(), vec.end(), 8), vec.end() );
```
### Как удалить из LIST списка все числа равные 8-ке
A. вызвать list.remove() и всё.
```
list.remove( list.begin(), list.end(), 8);
```
У std::list есть собственная реализация удаления объектов, list::remove и она быстрая.
- Элементы там не двигаются, а лишь переставляются ссылки.
- erase вызывать уже не нужно, потому что list::remove уничтожает ненужные элементы сам.

### Что быстрее std::set::find(10) или std::binary_search( std::vector_thst_is_sorted - begin(), end(), 10)
Ответ:
- При том, что поиск в std::set - O(log(n)) и Оценка скорости Алгоритма бинароного поиска тоже O(log(n))
- Быстрее окажется поиск по вектору, т.к. при поиске в std::set мы работаем с деревом, а в случае с вектором мы работаем с данными, которые расположены последовательно, что повышает эффективность для работы кэшэй процессора

### Проблема итераторов в std::vector
```
std::vector<int> vec();
auto it = vector.begin();
for(int i = 0; i < 100; ++i) {
  vec.push_back(i);
}
const int value = *it; // !! DANGER !
```
- Итератор на элемент в векторе может стать не валидным. Ибо во время вставки в векторе может произойти аллокация памяти, в результате чего все элементы будут размещены в совсем другом участке памяти.



### идиома RAII идиома RAII подход
raii - инициализация ресурса при создании объекта, удаление при освобождении объекта.

- 1) RAII — это аббревиатура, означающая Resource Acquisition Is Initialization.
- 2) СУТЬ: Программная идиома ООП, которая заключается в том, что с помощью конструктора и деструктора - **получение ресурса** неразрывно совмещается с **инициализацией**, а **освобождение ** ресурса — с **уничтожением объекта**.
		       
- Коротко: RAII объект (владение ресурсом передаём объекту на стеке, и этот объект уничтожает ресурс в своём деструкторе
- Подробно: Идиома RAII (Resource Acquisition Is Initialization) или Концепция RAII (Запрос ресурса при инициализации) СУТЬ: Создание и удаление объекта должно происходит на одном и том же "уровне", в рамках той же единицы трансляции / области видимости (частный случай - ресурс запрашивается в конструкторе и освобождается в декструкторе)

- СТАНДАРТ !!! C++ гарантирует вызов деструктора для объектов созданных на стеке (*), при выходе из области видимости (за текущий блок выполнения программы) в независимости от того, будет ли это изза сгенерированного исключения, вызов return, конец блока. 
- (*) RAII концепция работает, только в том случае, если наш RAII-объект создаётся на стеке, а не на куче.
Как уже было сказано ранее в языке С++ (в отличие от языка C#), при генерации исключения в конструкторе класса деструкторы уже сконструированных полей (т.е. подобъектов) будут вызваны автоматически. Это значит, что в данном случае вызов деструктора объекта Resource1 произойдет автоматически и никаких утечек ресурсов не будет.
- Суть идиомы RAII в том, что класс инкапсулирует владение (захват и освобождение) некоторого ресурса — например, открытого файлового дескриптора. Когда объекты-экземпляры такого класса являются **автоматическими переменными**, гарантируется, что когда они выйдут из области видимости, будет вызван их деструктор — а значит, ресурс будет освобождён.
- Поскольку деструктор локальной (автоматической) переменной вызывается при выходе её из области видимости, то ресурс гарантированно освобождается при уничтожении переменной. Это справедливо даже в ситуациях, в которых возникают исключения. 

### ГАРАНТИЯ ГАРАНТИИ ОТСУТСТВИЯ ИСКЛЮЧЕНИЙ ГАРАНТИИ БЕЗОПАСНОСТИ ИСКЛЮЧЕНИЙ
- Гарантия отсутствия исключений - noexcept - ни при каких обстоятельствах функция не генерирует исключ, а если генерирует, то они внутри неё же и отлавливаются
- Базовая гарантия безопасности исключений - если возникло исключение, то все объекты остаются в согласованном состоянии, утечек ресурсов не произошло
- Строгая гарантия безопасности исключений - если во время операции возникло исключение, то программа останется в том же состоянии, которое было до начала выполнения операции. (ДРУГИМИ СЛОВАМИ если при выполнении операции возникает исключение, то это не должно оказать какого-либо влияния на состояние приложения )

Проблемный код с потенциальным исключением, которое приводит к Утечке памяти
```
template<class T> struct Arr
{
 void resize(size_t n) {
   // *1 // new - std::bad_alloc - при нехватке памяти
   // НО мы ещё не успели изменить данные объекта Arr, и фактически Arr 
   // останется таким же как до запуска операции resize
   // *2 // У типа Т потенциально может быть конструктор, который
   // может бросить исключение, даже если его не бросит само new
   // (!) НО ЕСЛИ исключение происходит в конструкторе внутри оператора new
   // то компилятор гарантирует нам, что память будет освобождена
   // останется таким же как до запуска операции resize
   T* ndata = new T[n]; // *1 *2

   // *3 // оператор= присваивания тоже может бросить исключение,
   // и без try-catch и тогда для указателя ndata не вызовется delete и будет
   // УТЕЧКА ПАМЯТИ
   for(size_t i = 0; i!= n && i != size_; ++i)
     ndata[i] = data_[i]; // *3
	
   delete [] data_;
   data_ = ndata;
   size_ = n;
 }
 
 T* data_;
 size_t size_;
};

```

РЕШЕНИЕ 1
```
template<class T> struct Arr
{
 void resize(size_t n) {
   // *1 оставит всё согласованным и *2 память очистится компилятором
   T* ndata = new T[n];

   // *3 // оператор= присваивания может бросить исключение, и *ndata утечёт
   try {
     for(size_t i = 0; i!= n && i != size_; ++i)
       ndata[i] = data_[i]; // *3
   } catch (...) {
     delete [] data_;
	 throw;
   }
	
   delete [] data_;
   data_ = ndata;
   size_ = n;
 }
 
 T* data_;
 size_t size_;
};
```

РЕШЕНИЕ 2 - RAII объект (владение ресурсом передаём объекту на стеке, и этот объект уничтожает ресурс в своём деструкторе)
```
template<class T> struct Arr
{
 void resize(size_t n) {
   unique_ptr<T[]> ndata(new T[n]);

   // *1 // ЕСЛИ оператор= присваивания бросит исключение,
   // то начнётся размотка стека, и 
   // деструктор unique_ptr освободит указатель ndata
   for(size_t i = 0; i!= n && i != size_; ++i)
     ndata[i] = data_[i]; // *1
	
   data_ = std::move(ndata);
   size_ = n;
 }
 
 unique_ptr<T[]> data_;
 size_t size_;
};
```

РЕШЕНИЕ 3 - Проделать все опасные операции над копией объекта и если что-то пошло не так просто оставить копию и забить на выполнение операции



### Порядок создания и уничтожения объектов:

```
class A : public E, F, G
{
public:
	A() : __b(1), __c(2), __d(3) { }
	~A() { }

	// Порядок создания и удаления
	B __b; // 1() //3~()
	D __d; // 2() //2~()
	C __c; // 3() //1~()
	int _a = 0;
};

то есть:
 E()
 F()
 G()
 
 B()
 D()
 C()
 
 A()
 
 ~A()
 
~C()
~D()
~B()

~G()
~F()
~E()

```

### Свой класс исключений

```
class BaseException { public:
  BaseException() { printf("Base"); }
  ~BaseException() { printf("~Base"); }
};
class DerivedException : public BaseException
{ public:
  DerivedException() { printf("Derived"); }
  ~DerivedException() { printf("~Derived"); }
};
class SubException : public DerivedException
{ public:
  SubException() { printf("Sub"); }
  ~SubException() { printf("~Sub"); }
};

// ERROR
// ВСЕГДА ЛОВИТСЯ БУДЕТ catch (BaseException e)
// не важно какое исключение бросится Base / Sub / Derived

try { throw NumberException(x); }
catch (BaseException& e) { printf("catch for Base"); }
catch (SubException& sex) { printf("catch for SubException"); }
catch (DerivedException& dex) { printf("catch for DerivedException"); }
```
- Порядок catch() блоков для корректной работы исключений, имеющих какую-то иерархию, следующий:
```
try { throw NumberException(x); }
catch (MOST_CHILD_EX& last_child_ex) { ... }
...
catch (DERIVED_DERIVED_EX& ddex) { ... }
catch (DERIVED_EX& dex) { ... }
catch (BASE_EXCEPTION& dex) { ... }
```
- последний под-класс должен стоять в самом ПЕРВОМ catch, а Базовый класс-первый-родитель должен стоять в ПОСЛЕДНЕМ catch, иначе ловиться исключения будут неверно.

### Обработка двойного исключения

```
class ExitBoom
{
public:
  ExitBoom() { printf("ExitBoom()"); }
  ~ExitBoom() {
    printf("~ExitBoom");
    throw 2;
  }
};

void main() {
try  {
  ExitBoom bomb;
  throw 1;
  
  // throw 1; - бросает исключение, 
  // Это вызывет выход за единицу трансляции 
  // блока try{}, выходим за его фиг.скобки
  // Это вызывает уничтожение всех локальных 
  // объектов, в частности уничтоженеие объекта bomb
  // вызывается его деструктор и бросается throw 2;
  // 2 исключения в одном блоке try приведут к
  // terminate -> и abort() has been called
}
catch (...) {
  printf("catch");
}
}
```
- ЕСЛИ в Блоке try происходит более одного исключения, то это приводит к вызову terminate()


### Исключение в Конструкторе / Исключения в Конструкторе / Исключения в Конструкторах / Конструктор и исключение
Вот что написано в стандарте С++ 2011 пункт 15.2.2

An object of any storage duration whose initialization or destruction is terminated by an exception will have destructors executed for all of its fully constructed subobjects (excluding the variant members of a union-like class), that is, for subobjects for which the principal constructor (12.6.2) has completed execution and the destructor has not yet begun execution. Similarly, if the non-delegating constructor for an object has completed execution and a delegating constructor for that object exits with an exception, the object’s destructor will be invoked. If the object was allocated in a new-expression, the matching deallocation function (3.7.4.2, 5.3.4, 12.5), if any, is called to free the storage occupied by the object.

- Итак: Бросаем исключение в конструкторе
- **Помним 1-ое** - В языке C++ объект считается созданным только в тот момент, когда его конструктор завершит выполнение без ошибок. Здесь этого не происходит, поэтому объект никогда не был создан, а, значит, и память под него "как бы" не выделялась.
- **Помним 2-ое** - Согласно стандарту языка с++, при выбросе исключения в конструкторе, будет происходить расскрутка стека, в процессе которой будут вызваны деструкторы переменных класса (ошибочно называемых "полями"), который бросил исключение.
- ТОЧНЕЕ: Компилятор обязан корректно освободить память, в которой будет находится объект. В случае исключения в его конструкторе, вызываются деструкторы всех уже созданных объектов, в следующем порядке: 
- Деструктор самого класса НЕ вызовется (ибо конструктор не отработал полностью)
- НО Вызовутся деструкторы всех ЧЛЕНОВ класса (что были созданы на стеке) с самого нижнего до самого верхнего
- Далее классы в строке НАСЛЕДОВАНИЯ С права налево, согласно строке наследования
Тем самым очистка всех "автоматических" объектов выполнится корректно и утечёт только память для тех объектов, которые будут выделены в куче, через оператор new, ибо не факт, что для них будет вызван их delete (ведь скорее всего вызов delete для них как раз был бы в деструкторе класса, в конструкторе которого бросится исключение)
- **Помним 3-тье** - TRY-CATCH-Блок: Когда сгенерировано исключение, все нединамически созданные к этому времени в блоке try объекты уничтожаются.

- **Помним 4-тое** - **NEW и исключение в конструкторе**
- БАЗОВАЯ гарантия - если не будет никаких утечек ресурсов и объект останется в **СОГЛАСОВАННОМ состоянии**
- **new** - предоставляет **СТРОГУЮ гарантию** безопасности исключений. **ТРАНЗАКЦИОННОСТЬ** - при исключении, программа останется в том же состоянии, которое было до начала выполнения операции в которой случилось исключение.
- ЕСЛИ мы для NEW 1. malloc выделили память и 2. ctor() выбросил исключение, ТО new гарантирует нам, освобождение этой памяти
	
```
class A { A() { throw 1; } };
void main(){
  try {
    // new: 1. malloc
    // new: 2. call ctror() of class
    A* a = new A; // Утечки НЕ БУДЕТ
    delete a; // Хотя и до delete a мы не дойдём
  }
  catch(..) {
    // new гарантирует нам освобождение памяти
  }
}
```

// Что выведется на экран :	
```
struct A0
{
  A0() { 
  	printf("_A0"); }
  virtual ~A0() {
  	printf("_~A0");
  }
};
struct A
{
  A() {
  	printf("_A"); }
  virtual ~A() {
  	printf("_~A");
  }
};

struct C
{
  C() {
  	printf("_C");
  }
  virtual ~C() {
  	printf("_~C");
  }
};
struct CC
{
  CC() {
  	printf("_CC");
  }
  virtual ~CC() {
  	printf("_~CC");
  }
};
struct CCC
{
  CCC() {
  	printf("_CCC");
  }
  virtual ~CCC() {
  	printf("_~CCC");
  }
};

class D
{
public:
  D() {
  	printf("_D");
  }
  ~D() {
  	printf("_~D");
  }
};
class E
{
public:
  E() {
  	printf("_E");
  }
  ~E() {
  	printf("_~E");
  }
};
class F
{
public:
  F() {
  	printf("_F");
  }
  ~F() {
  	printf("_~F");
  }
};


struct B : public A, CCC, CC
{
  B() {
  	printf("_B");
  	__f = new F;
  	throw 1;
  }
  ~B() {
  	printf("_~B"); // из-за "trow 1;" не вызовется
  	delete __f; // из-за "trow 1;" не вызовется
  }

  D __d; // 1() //4~()
  C __c; // 2() //3~()
  E __e; // 3() //2~()
  F* __f; // 4() //1 *__f = 0;
};


int main()
{
  // ИСКЛЮЧЕНИЕ В КОНСТРУКТОРЕ
  // ИСКЛЮЧЕНИЕ БРОСАЕТ КОНСТРУКТОР
  // B() { throw 1; }

  /*
  // EXAMPLE 1:
  {
  	A*  test = new B; // (**) C ИСКЛЮЧЕНИЕМ
  	// _A_CCC_CC_D_C_E_B_F
  }
  //*/

  //*
  // EXAMPLE 2:
  try
  {
  	A0* a0 = new A0;
  	A*  aa = new A;
  	//B b; // (*) БЕЗ ИСКЛЮЧЕНИЯ
  	A*  ab = new B; // (**) C ИСКЛЮЧЕНИЕМ
  }
  catch (...)
  {
  	1;
  	//return 1;
  }
  //*/

  // (*) БЕЗ ИСКЛЮЧЕНИЯ В ~B() было бы так:
  // _A0_A_A_CCC_CC_D_C_E_B_F _~B_~F_~E_~C_~D_~CC_~CCC_~A

  // _A0
  // _A
  // B b;
  // СОЗДАНИЕ:
  // Наследники, Члены, сам Класс
  // сначала классы в строке НАСЛЕДОВАНИЯ _A _CCC _CC (от B)
  // в порядке объявления слева направо
  // Далее ЧЛЕНЫ класса с первого верхнего _D _C _E
  // Завершится всё конструктором самого   _B (+ _F внутри него)
  //
  // УДАЛЕНИЕ: 
  // Cам класс, Члены, Наследники
  // Начинается всё деструктором самого    _~B (+ _~F внутри него)
  // сначала ЧЛЕНЫ класса с самого нижнего _~E _~C _~D_
  // Далее классы в строке НАСЛЕДОВАНИЯ    _~CC _~CCC _~A
  // С права налево

  // (**) C ИСКЛЮЧЕНИЕМ
  //_A0_A_A_CCC_CC_D_C_E_B_F _~E_~C_~D_~CC_~CCC_~A

  // _A0
  // _A
  // A*  ab = new B;
  // _A_CCC_CC_D_C_E_B_F _~E_~C_~D_~CC_~CCC_~A
  // Деструктор самого класса _~B не вызовется (как и + _F внутри него),
  // конструктор его не отработал, формально объект не создался, 
  // НО
  // деструкторы Членов класса (снизу вверх)
  // и Наследников вызовутся как обычно (справа налево),
  //*/

  //* // SOLUTION_1
  try
  {
  	std::unique_ptr<A0> a0(new A0());
  	std::unique_ptr<A> aa(new A());
  	std::unique_ptr<B> ab(new B());
  }
  catch (...)
  {
  	//return 1;
  	1;
  }
  
  // _A0 _A _A_CCC_CC _D_C_E _B_F _~E_~C_~D _~CC_~CCC_~A _~A _~A0

  //*/
}
```

### Исключение в Деструкторе / Исключения в Деструкторе / Исключения в Деструкторах / Деструктор и исключение
https://academy.yandex.ru/handbook/cpp/article/exceptions
- **Помним 1-ое** - В процессе обработки исключения, исключение не может исходить из самого деструктора, так как это считается ошибкой в работе механизма обработки исключений, приводящей к вызову std::terminate(); Однако деструктор может обрабатывать исключения, если написать внутри него блок try{}catch{}, которые генерируются вызываемыми им функциями, сам же деструктор не должен возбуждать исключений. Если быть точнее, то выброшенное исключение не должно покинуть фигурные скобки деструтора, ибо это приведёт к terminate()
- Введя деструктор ~A(), компилятор неявно пометит его как "~A() noexcept(true)" - по умолчанию. Естветсвенно вызов исключения в нём приведёт к terminate()
- **Помним 2-ое** - Если мы хотим бросать из него исключения, то нам нужно пометить его специально как "~A() noexcept(false)" и к terminate() вызов исключения в нём уже более не приведёт
- **Помним 3-тье** - Двойное исключение - Если мы разрешаем объекту бросать исключение из деструктора, через пометку ~A() noexcept(false), то мы должны быть готовы к тому чтобы понимть про случай **Обработка двойного исключения** (смотри 2 вопросами выше). СУТЬ: ЕСЛИ в Блоке try происходит более одного исключения, то это приводит к вызову terminate().  
```
try  {
  ExitBoom bomb;
  throw 1;
  // 1. throw 1
  // 2. деструктор bomb, а в нём ещё один throw 2;
  // 3. => terminate
}
catch (...) {
  printf("catch");
}
}
```

ОСТАЛЬНЫЕ ПРИМЕРЫ:
```
Написав код
class A
{
public:
//компилятор неявно добавит ~A() noexcept(true)
  ~A() {
    throw 1; // terminate() / message "abort() has been called"
  }
};

И вызвав
try {
  A a;
}
catch (...) {
  int c = 0;
}

ПРЕДУПРЕЖДЕНИЕ В некоторых компиляторах мы 
получим вывод с предупреждениями:

1>app.cpp(11): warning C4297: 
'A::~A': function assumed not to throw 
         an exception but does
		 
1>app.cpp(11): note: destructor or deallocator 
has a (possibly implicit) 
non-throwing exception specification
```
- Случится terminate(), даже не смотря на наличие try{}catch{}-БЛОКА, ибо к деструктору неявно допишется noexcept(true) и будет "~A() noexcept(true)", что будет вызывать terminate для любого исключения, которое выбросится за пределы функции деструтора.

Позволяем деструктору бросать исключения:
```
Написав код
class A
{
public:
   // компилятор неявно добавляет к деструтору ~A() noexcept(true)
   // Но мы ЯВНО напишем noexcept(false), чтобы деструктор МОГ бросать исключения:
  ~A() noexcept(false) {
    throw 1; // terminate() не случится ->
  }
};

И вызвав
try {
  A a;
}
catch (...) { // -> мы войдём в блок catch
  int c = 0;
}
```
- terminate() не случится, но и деструктор объекта не отработал полностью, так что вопрос с тем что произошло с объектом остаётся открытым

**НЮАНС НОМЕР 2**
- Базовые классы для класса у которого мы определяем ~() noexcept(false), тоже должны иметь приписку noexcept(false)
```
struct B : public A, CCC, CC
{
  B() {
  printf("_B");
  }
  ~B() noexcept(false) {
    printf("_~B"); // из-за "trow 1;" не вызовется
    throw 1;
  }
}

ОШИБКИ КОМПИЛЯЦИИ:  
Error C2694 'B::~B(void) noexcept(false)': 
overriding virtual function has less restrictive 
exception specification than base class 
virtual member function 'A::~A(void)'

Error C2694 'B::~B(void) noexcept(false)': 
--..-- 'CCC::~CCC(void)'
Error C2694 'B::~B(void) noexcept(false)':
--..-- 'CC::~CC(void)'
```

ПОСЛЕДНИЙ ПРИМЕР:
```
//Что выведется на экран :
struct A0
{
  A0() { 
  	printf("_A0"); }
  virtual ~A0() {
  	printf("_~A0");
  }
};
struct A
{
  A() {
  	printf("_A"); }
  virtual ~A() noexcept(false) {
  	printf("_~A");
  }
};

struct C
{
  C() {
  	printf("_C");
  }
  virtual ~C() {
  	printf("_~C");
  }
};
struct CC
{
  CC() {
  	printf("_CC");
  }
  virtual ~CC() noexcept(false) {
  	printf("_~CC");
  }
};
struct CCC
{
  CCC() {
  	printf("_CCC");
  }
  virtual ~CCC() noexcept(false) {
  	printf("_~CCC");
  }
};

class D
{
public:
  D() {
  	printf("_D");
  }
  ~D() {
  	printf("_~D");
  }
};
class E
{
public:
  E() {
  	printf("_E");
  }
  ~E() {
  	printf("_~E");
  }
};
class F
{
public:
  F() {
  	printf("_F");
  }
  ~F() {
  	printf("_~F");
  }
};


struct B : public A, CCC, CC
{
  B() {
  	printf("_B");
  	__f = new F;
  	//throw 1;
  }
  ~B() noexcept(false) {
  	printf("_~B");
  	delete __f;
  	throw 2;
  }

  D __d; // 1() //4~()
  C __c; // 2() //3~()
  E __e; // 3() //2~()
  F* __f; // 4() //1 *__f = 0;
};

int main()
{
  // ИСКЛЮЧЕНИЕ В ДЕСтрукторе
  // ИСКЛЮЧЕНИЕ БРОСАЕТ ДЕСтруктор
  // ~B() { throw 2; }

  // EXAMPLE 2:
  try
  {
  	A0* a0 = new A0;
  	A*  aa = new A;
  	B b; // (**) C ИСКЛЮЧЕНИЕМ В ДЕСтрукторе
  }
  catch (...)
  {
  	return 1;
  }
  
  /*
  _A0 _A _A_CCC_CC _D_C_E _B_F _~B_~F _~E_~C_~D _~CC_~CCC_~A
  
  // _A0
  // _A
  // B b;
  // СОЗДАНИЕ:
  // Наследники, Члены, сам Класс
  // сначала классы в строке НАСЛЕДОВАНИЯ _A _CCC _CC (от B)
  // в порядке объявления слева направо
  // Далее ЧЛЕНЫ класса с первого верхнего _D _C _E
  // Завершится всё конструктором самого   _B (+ _F внутри него)
  //
  // УДАЛЕНИЕ: 
  // Cам класс, Члены, Наследники
  // Начинается всё деструктором самого    _~B (+ _~F внутри него)
  // сначала ЧЛЕНЫ класса с самого нижнего _~E _~C _~D_
  // Далее классы в строке НАСЛЕДОВАНИЯ    _~CC _~CCC _~A
  // С права налево
  */
}
```

### Разница между abort() и exception()
- abort() -> сразу terminate();
- exception() -> если оно не влетело в try{}catch(){} блок, то по сути это тот же самый abort()
- (!) abort() - Деструкторы для атоматических переменных, для abort() не вызовутся.
- (!) exception() - Деструкторы для атоматических переменных, начинают вызываться ТОЛЬКО, ТОГДА, КОГДА мы попали в catch-БЛОК и вот тогда начинается stack-unwinding Раскрутка стека.

### Что происходит, когда мы делаем fork()

### Свой класс строки
- иммутабельный (будем редко их менять) - можно на основе std::array
- достаточно часто изменяем строки, но операции вставки и удаления чаще всего в конец, тогда сделаем на основе std::vector
- достаточно часто изменяем строки, но операции вставки и удаления чаще всего в середину, тогда сделаем на основе std::list (возможно std::deque)
- copy on write строки

### CString - Copy-On-Write (COW)
- Технология позволяет эффективнее работать с временными объектами (ибо мы не копируем объект сразу, а лишь тогда когда объект изменится)
- (создали копию, ориг=буфер, ориг_и_копия шарят буфер, как только кто-то буфер начинает меняться, копия РЕАЛЬНО скопирует содержимое буфера перед его изменением к себе) При копировании на самом деле ничего не копируется по началу, копия и оригинал просто указывают в одну и ту же память, как только кто-то решит внести изменение в оригинал или в копию, вот тогда, копия РЕАЛЬНО выполнит копирование и перестанет ссылаться на оригинал.
- По технологии Copy-On-Write вместо того, чтобы строка уникально владела динамическим объектом, который создан в куче, можно сделать так, чтобы несколько строк одновременно ссылались на динамический объект. Тогда в этом динамическом объекте будет счетчик ссылок — сколько одновременно строчек работает с ним. Код будет выглядеть так:
- Только старый Windows CString класс последним использовал COW-технологию 
- Начиная с C++11 COW строчки запрещены в стандарте. 
Подробнее тут: https://habr.com/ru/company/oleg-bunin/blog/352280/

```
class string_impl {
  char* data;
  size_t size;
  size_t capacity;
  size_t use_count; // <===
  // ...
};
```

### std::string - Работа с маленькими строками 
### SSO - Short String Optimsation - Small String optimisation
### SBO - Small Buffer Optimization

- Если строка маленькая, ТО она в HEAP не сохраняется, не делается лишняя аллокация памяти, короткая строка (пустая, или 22 символа) будет хранится внутри спецального union-а, в котором есть специальный массив small_buffer_t { char data[sizeof(no_small_buffer_t)]; }
- строка нулевой длинны не будет храниться в куче и не будут динамически аллоцироваться
Подробнее тут: https://habr.com/ru/company/oleg-bunin/blog/352280/

```
class string {
  size_t capacity;
  union {
    struct no_small_buffer_t {
      char* data;
      size_t size;
    } no_small_buffer;
    struct small_buffer_t {
      char data[sizeof(no_small_buffer_t)];
    }small_buffer;
  }impl;
}
```


====================================================	

# Undefined Behaviare Неопределённое поведение неопределенное поведение

**Стандарт: Неопределенное поведение** (undefined behavior) – поведение ф-ции или программы, которое не описано в стандарте вовсе или явно. Поведение, которое может возникать в результате использования ошибочных программных конструкций или некорректных данных, на которые Международный Стандарт не налагает никаких требований. Неопределенное поведение также может возникать в ситуациях, не описанных в Стандарте .

**Стандарт:** *Точки следования* (sequence points)– такие точки в процессе выполнения программы, в которых все побочные эффекты уже выполненного кода закончили свое действие, а побочные эффекты кода, подлежащего исполнению, еще не начали действовать. (§1.9/7)

**Стандарт:** *Побочный эффект* (side effect) – результат доступа к volatile объекту, изменения объекта, вызова функции из библиотеки I/O или же вызова функции, включающей в себя какие-то из этих действий. Побочный эффект является изменением состояния среды выполнения.

**Стандарт:** *Неуточняемое поведение* (unspecified behavior) – поведение, для которого Стандарт предлагает два или более возможных вариантов и не налагает четких требований на то, какой из них должен быть выбран в определенной ситуации.

**Стандарт:** *Поведение, определяемое реализацией* (implementation-defined behavior) – поведение правильно сформированной программной конструкции с правильными данными, которое зависит от реализации (должно быть документировано для каждой реализации).


### Undefined Behaviare Пример

Весь список: 

https://www.nayuki.io/page/undefined-behavior-in-c-and-cplusplus-programs

Примеры UB (Undefined Behaviour) / Не поеределённого поведения:

1) Порядок работы с выражениями аргументов функции оставлено на усмотрение компилятора:

1.1) int i = 1;  printf("%d, %d\n", i++, i++);

// порядок работы с выражениями аргументов функции 
оставлено на усмотрение компилятора. Поэтому код 
вполне может распечатать как "1, 2", так и наоборот "2, 1"

1.2) int z = x++ + y++; 
```
// т.к.
// int operator + (int left, int right)
// оператор + это тоже функция с параметрами

// не известно, будет вычислен первым x++ или y++
Стандарте сказано: Точки следования (sequence points) 
– такие точки в процессе выполнения программы, 
в которых все побочные эффекты уже выполненного 
кода закончили свое действие, 
а побочные эффекты кода, подлежащего исполнению, 
еще не начали действовать. (§1.9/7)

В §5/4 Стандарт говорит: Между двумя точками следования 
скалярный объект должен менять хранимое значение 
при вычислении выражения не более одного раза.
```

1.3) i++ * ++i;
```
i = ++i;   // 
++i = 2;   // i изменено более 1 раза
i = ++i +1 // 
i = (i,++i,++i); 
// нет точки следования между 
//правым `++i` и присвоением `i`
```

1.4) Порядок вызова функций.
```
int Hello() { return printf("Hello"); }
int World() { return printf("World !"); }
int main() { int a = Hello() + World(); /**может вывести «Hello World!» или «World! Hello»
                             ^
                             | 
   Функции могут быть вызваны в любом порядке **/
   return 0;
} 
// ибо
// int operator + (int left, int right)
// оператор + это тоже функция с параметрами
```

2) Не проинициализированная переменная
```
bool p; 
if(p) // UB access to uninitialized scalar
    std::puts("p is true");
if(!p) // UB access to uninitialized scalar
    std::puts("p is false");
```

3) Деление на НОЛЬ
- double z = z / 0.0; unsigned z = z / 0;
4) Выход за пределы массива через operator[], v={1,2,3}; v[4]; / v[-1]; => UB
5) Переполнение (overflow) знакового int (signed integer type) Всегда UNDEFINED BEHAVIOR (UB):
- int x = INT_MAX; x++; // Boom! (usually silent)
6) Разыменование нулевого указателя. 
- int *p = 0; *p = 123; // Kaboom
7) Побитовый сдвиг за пределы числа бит в типе влева или впарво (Shifting more than the integer width or less than zero is UB):
```
uint32_t x = 0;
x = x << 33;  // Blamo
x = x >> (-1);  // Kapow
```
8) return в функции, без вовращаемого значения.
```
bool myfun() { return; }
int main()
{ bool ret = myfun(); }
```

Многопоточные UB:

10) 3 потока, 1,2,3 -> 3-взял мьютекс, 2_и_1 стали ждать, Номер 3 освободил мьютекс. 
- Захватить следующим мьютекс может как 1 так и 2 или как 2 так и 1 порядок не определён.
11) Два раза захваченный мьютекс:
- **mutex m; m.lock(); m.lock();**



====================================================	
	
# МНОГОПОТОЧНОСТЬ

https://habr.com/ru/post/443406/

Топ 20 ошибок при работе с многопоточностью на С++ и способы избежать их

0) В мьютексы не встроено понятие Очереди освобождения потоков.
- Управление получает не обязательно первый, кто обратился (когда поток встал на ожидание)
- 3 потока, 1,2,3 -> 3-взял мьютекс, 2_и_1 стали ждать, 3 освободил мьютекс. захватить следующим мьютекс может как 1 так и 2 или как 2 так и 1 порядок не определён.
1) Не использовать join() / detach()
- ПРОБЛЕМА: ибо внутри ~thread() сработает terminate();
2) thread t1; // [1] t1.detach(); [2] КОД [3] t1.join(); // CRASH !!!
- НЕ ЗАБЫВАЕМ ПЕРЕД join() СДЕЛАТЬ joinable() => if (t1.joinable()) { t1.join(); }
3) thread::join() блокирует вызывающий поток выполнения
4) Не забываем std::ref(), если хотим передать в поток значение ПО ССЫЛКЕ.
- Просто передача в функцию параметра, даже если функция func(&x) принимает что-то по ссылке по факту std::thread без std::ref() передаст в неё только значение и всё
5) Разделяемые данные и ресурсы ЗАЩИЩАЕМ с помощью критической секции (например мьютексом), когда МНОГО потоков конкурируют за ресурсы и общие данные.
- 6.1) Можно забыть mutex.unlock, поэтому используем классы RAII-обёртки, lock_guard(), unique_lock(), scoped_lock(), std::lock(m1, m2, ..., mN);
- 6.2) Можно вызвать на мьютексе лишний lock(),
- ПРОБЛЕМА: ДВОЙНОЙ лок mutex.lock(); mutex.lock(); - UNDEF_BEHAVIOUR. На разных платформах может ЗАВИСНУТЬ (Linux), а может и CRASH (Win, но врятли) быть
- РЕШЕНИЕ: try_lock() а так же std::adopt_lock, чтобы показать объектам std::lock_guard, что мьютексы уже заблокированы
- 7.1) Делать размер критической секции больше, чем это необходимо.
- 1 поток захватил мьютекс на минуту, 10 других потоков, что пытаются в нее войти, по сути заблокированы
- 7.2) Держим в критической секции ТОЛЬКО те методы, что МОДИФИЦИРУЮТ данные
8) ДЕДЛОК - Взятие нескольких блокировок в разном порядке
- РЕШЕНИЕ_1: std::scoped_lock lock{muA, muB};
- РЕШЕНИЕ_2: std::timed_mutex, в котором можно указать таймаут, по истечении которого блокировка будет снята, если ресурс не стал доступен.
- РЕШЕНИЕ_3: std::lock(lock1, lock2); // Передаём НЕ мьютексы, а два юник-лока
``` 
std::unique_lock<std::mutex> lock1(from.m, std::defer_lock);
std::unique_lock<std::mutex> lock2(to.m, std::defer_lock);
// блокирует оба объекта unique_lock без взаимной блокировки
std::lock(lock1, lock2); 
```
9) Пытаться дважды захватить блокировку std::mutex
- В разных местах std::lock_guard<std::mutex> lock(mu); на один и тот же мьютекс
10) Использовать мьютексы, когда достаточно std::atomic типов
- ПРОБЛЕМА int counter; mu.lock(); counter++; mu.unlock();
- РЕШЕНИЕ std::atomic<int> counter; ... counter++;
11) Создавать и разрушать большое количество потоков напрямую, вместо использования пула свободных потоков
- **Создание и уничтожение потоков – дорогостоящие операции** с точки зрения использования процессорного времени.
- РЕШЕНИЕ - создании пула предварительно выделенных потоков, которые могут обрабатывать рутинные задачи
- thread oversubscription (ситуация, в которой количество потоков превышает количество доступных ядер и значительная часть процессорного времени тратится на переключение контекста [прим. переводчика]). Это может повлиять на производительность системы.
12) Не обрабатывать исключения, возникающие в фоновых потоках
- ПРОБЛЕМА: Исключения, выброшенные в одном потоке, не могут быть обработаны в другом потоке.
- РЕШЕНИЕ: std::exception_ptr применяется для обработки исключения, выброшенного в фоновом потоке.
13) Использовать потоки для симуляции асинхронной работы, вместо применения std::async
- ПРОБЛЕМА: Kurt Guntheroth обнаружил, что с точки зрения производительности, накладные расходы на создание потока в 14 раз больше, чем использование async.
- ПРОБЛЕМА: Если вам нужно, чтобы код выполнился асинхронно, т.е. без блокировки основного потока выполнения
- РЕШЕНИЕ_1: использование std::async().
- РЕШЕНИЕ_2: std::future(), std::promise. - получить результат выполнения асинхронной операции обратно в вызывающий поток с помощью std::future объекта. std::future asyncResult2 = std::async(&ConjureMagic);
14) Не использовать std::launch::async если требуется асинхронность
- ПРОБЛЕМЫ:
- Функция **std::async()** носит не совсем корректное название, потому что **по умолчанию может выполняться НЕ АСИНХРОННО**!
- Есть две политики выполнения std::async:
- std::launch::async: переданная функция начинает выполняться незамедлительно в отдельном потоке
- std::launch::deferred: переданная функция не запускается сразу же, ее запуск откладывается до того как будут произведены вызовы get() или wait() над std::future объектом, который будет возвращен из вызова std::async. В месте вызова этих методов, функция будет выполняться синхронно.
- Когда мы вызываем std::async() с параметрами по умолчанию, происходит запуск с комбинацией этих двух параметров, что в действительности приводит к непредсказуемому поведению. Существует ряд других сложностей, связанных с использованием std:async() с политикой запуска по умолчанию: 
- РЕШЕНИЕ: Для избежания всех этих сложностей всегда вызывайте std::async с политикой запуска std::launch::async.
- КОРОЧЕ: //выполнение функции myFunction используя std::async с политикой запуска по умолчанию **НЕ ДЕЛАЙТЕ ТАК auto myFuture = std::async(myFunction);**
- (!!!) Вместо этого **ДЕЛАЙ ВСЕГДА ТАК**: 
- //выполнение функции myFunction асинхронно
- **auto myFuture = std::async(std::launch::async, myFunction);**
15) Вызывать метод get() у std::future объекта в блоке кода, время выполнение которого критично
16) Непонимание того, что исключения, выброшенные внутри асинхронной операции, передадутся в вызывающий поток только при вызове std::future::get()
- РЕШЕНИЕ std::packaged_task и переместить его в нужный поток выполнения после установки свойств потока.
17) Создавать намного больше «выполняющихся» потоков, чем доступно ядер
- ПРОБЛЕМА - количество выполняющихся потоков поддерживает система? Используйте метод thread::hardware_concurrency().
18) Использование ключевого слова volatile для синхронизации
- ПРОБЛЕМА: Ключевое слово volatile перед указанием типа переменной не делает операции с этой переменной атомарными или потокобезопасными.
- РЕШЕНИЕ: То, что вы, вероятно, хотите, это std::atomic.
19) Использование Lock Free архитектуры, кроме случаев когда это совершенно необходимо
ПРОБЛЕМЫ И ВОПРОСЫ:Перед тем как задумываться над применением подхода разработки без блокировок, пожалуйста ответьте на три вопроса:
- Пробовали ли вы спроектировать архитектуру вашей системы таким образом, чтобы она не нуждалась в механизме синхронизации? Как правило, лучшая синхронизация– отсутствие синхронизации.
- Если вам нужна синхронизация, профилировали ли вы свой код для понимания характеристик производительности? Если да, пытались ли вы оптимизировать узкие места?
- Можете ли вы горизонтально масштабироваться вместо того чтобы масштабироваться вертикально?

### Дополнительно о mutex.lock + mutex.lock
ДОКУМЕНТАЦИЯ:
- Locks the mutex. If another thread has already locked the mutex, a call to lock will block execution until the lock is acquired.
- If lock is called by a thread that already owns the mutex, (** mutex.lock + mutex.lock **) this is an **UNDEFINED BEHAVIOR**: for example, the program may deadlock. An implementation that can detect the invalid usage is encouraged to throw a std::system_error with error condition resource_deadlock_would_occur instead of deadlocking.

### Дополнительно о std::lock(...Args) - ЕСТЬ В С++11
ДОКУМЕНТАЦИЯ:
- Locks the given Lockable objects lock1, lock2, ..., lockn using a deadlock avoidance algorithm to avoid deadlock.
- The objects are locked by an **unspecified series of calls to lock, try_lock, and unlock**. If a call to lock or unlock results in an exception, unlock is called for any locked objects before rethrowing.
- (!) std::lock() это ОЛДСКУЛ (НО зато он есть в С++11), лучше ЮЗАТЬ **std::scoped_lock** (Но он только с С++17) offers a RAII wrapper for this function, and is generally **preferred to a naked call to std::lock**. 

### Дополнительно о std::scoped_lock(...Args) - НЕТ В С++11, только в С++17
ДОКУМЕНТАЦИЯ:
- Едиснвтенный минус ЕГО НЕТ В С++11, ОН ЕСТЬ только в С++17, Альтернатива в с++11 это std::lock(...Args)
	
# МНОГОПОТОЧНОСТЬ

### std::thread			  
- std::thread - поток НЕЛЬЗЯ КОПИРОВАТЬ, но можно МУВИТЬ, std::move() для потока допускается.
- У каждого потока свой - СТЕК
- Внутри потока шарится - HEAP, КОД, ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ

### Что будет, если Два потока выводят одновременно через std::cout
- std::cout - стандартный поток вывода - stdin, stdout, stderror - это файл (файловые дескрипторы). Гонки потоков для доступа к БУФЕР-у файла std::cout и вывода
- Для решения прорблемы потребуется mutex: который выполнит и добавление lock_guard<mutex> в то место функции, перед обращением к std::cout

### Что будет, если std::thread упадёт ДО join()
- Вызовется terminate() и программа упадёт

### Что будет, если создать std::thread t; и не написать t.join();
### Что будет, если создать std::thread t; и не написать t.detach();
- основной тред закончится, выйдем из main, вызовется деструктор созданного объекта потока std::thread tr(); и вызовется terminate.

### std::mutex:
- Когда мы выполняем lock (напрмиер lock_guard<mutex> lock(m1); lock_guard<mutex> lock(m2); ) двух мьютексов, то есть вероятность дедлока
- РЕШЕНИЕ_1 - вызывать локи последовательно (В ПРАВИЛЬНОМ ПОРЯДКЕ)
- РЕШЕНИЕ_2 - std::scoped_lock - чтобы атомарно залочить 2 мьютекса
- "+" и "-" выполняют 3 операции. 1.загрузку значения 2. увеличение 3. вовзрат результата
- дважды вызовем mtx.lock(); mtx.lock(); - UNDEBINED_BEHAVIOUR

### Синхронизация потоков:
- { mtx.lock / общий ресурс / mtx.unlock } - Мьютекс должен быть выше самих потоков, чтобы быть доступным и потоку один и потоку 2.
- Или { lock_guard<mtx> guard / общий ресурс / выйдем } - lock_guard лучше чем lock/unlock
- std::lock_guard - RAII - будет заблокирован только один раз при построении и разблокирован при уничтожении.
- lock_guard: когда объект создан, он пытается получить мьютекс (вызывая lock()), а когда объект уничтожен, он автоматически освобождает мьютекс (вызывая unlock())
- unique_lock - можно заблокировать и разблокировать более одного раза
- unique_lock: в отличие от lock_guard, также поддерживает отложенную блокировку, временную блокировку, рекурсивную блокировку и использование условных переменных
- scoped_lock - RAII - то, что пришло на замену lock_guard для работы с несколкими мьютексами
```
std::scoped_lock lock(e1.m, e2.m); // Коротко и удобно
// Запись с scoped_lock эквивалентна коду с использованием std::lock и std::lock_guard:
// std::lock(e1.m, e2.m); // коротко, но старо
// Длинно, много, некомпактно
// std::lock_guard <std::mutex> lk1 (e1.m, std::accept_lock);
// std::lock_guard <std::mutex> lk2 (e2.m, std::accept_lock);
```
	
- ЕЩЁ Мьютексы:
- ДВОЙНОЙ лок mutex.lock(); mutex.lock(); - UNDEF_BEHAVIOUR
- spin_lock - механизм организации потоков, который использует атомарную переменную и в цикле постоянно крутится до наступления какого-то события. Ложка с активным ожиданием.
- spin_lock - примитив синхронизации, для задач, для которых не нужно переключение контекста, то есть время ожидания для задач если приблизительно заранее изаестно, что оно мало, то мы можем использовать spin_lock в режиме ожидания, до тех пор пока не наступит освобождение ресурса, чтобы дождадться освобождения синхронизирущего элемента и при этом мы продолжим работать, и контекст при этом у нас не был переключён, ресурсы на перключение контекста не затратили.
- dead_lock - взаимная блокировка, когда два или более мьютекса захватили ресурсы, необходимые друг другу и вынуждены перекрёстно ждать освобождения этих ресурсов
- std::cond_variable (wait(), notify_one(), notify_all() ) - переменная на которой ожидают потоки, на которой можно поставить условие, что-бы пронотифаить смену своего состояния причём для одно потока, для последовательно несколких потоков или для всех потоков сразу, которые ожидают на этой переменной
```
std::unique_lock lk(m);
cv.wait(lk, []{return processed;});
```

**Семафор**
-  Есть Число потоков для работы с ресурсом. Счётчик семафора уменьшается на число потоков, которое обратилось к ресурсу, как только счётчик ==0, то остальные потоки должны ждать когда счётчик снова станет положительным, что будет означать, что следующий поток может обратиться к ресурсу.
- примитив синхронизации, разрешающий конкретному числу потоков одновременную работу c неким общим ресурсом. Для него задаётся 2 параметра, сколько сколько потоков могут получить доступ к ресурсу одновременно, и каково общее число потоков, которое начнёт параллельную работу. В нём есть счётчик 
std::counting_semaphore, std::binary_semaphore

### Разница между mutex и семафором?
- mutex - **lock() / unlock()** - строится на взаимных блокировках потоков, используя
- семафором - **wait() / notify() на condition_variable** - и соотвественно  строится на сигнализации свободности ресурсов, используя condition_variable и соотвественно wait() / notify()

- Мьютекс - средство **БЛОКИРОВКИ** объекта единственным потоком. Разблокировать мьютекс должен тот же поток, что его и блокировал.
- Семафор - это средство передачи **СИГНАЛОВ** между потоками. Один поток сигналит(семафорит), а другой эти события ловит.
	
### спинлок своими руками spinlock spin_lock поменьше

- https://gist.github.com/twoz/b446fa884384c4cfc6ca
```
class SpinLock {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        while (locked.test_and_set(std::memory_order_acquire)) { ; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};
```	

### спинлок своими руками spinlock spin_lock побольше

- https://rigtorp.se/spinlock/

```
struct spinlock {
  std::atomic<bool> lock_ = {0};

  void lock() noexcept {
    for (;;) {
      // Optimistically assume the lock is free on the first try
      if (!lock_.exchange(true, std::memory_order_acquire)) {
        return;
      }
      // Wait for lock to be released without generating cache misses
      while (lock_.load(std::memory_order_relaxed)) {
        // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
        // hyper-threads
        __builtin_ia32_pause();
      }
    }
  }

  bool try_lock() noexcept {
    // First do a relaxed load to check if lock is free in order to prevent
    // unnecessary cache misses if someone does while(!try_lock())
    return !lock_.load(std::memory_order_relaxed) &&
           !lock_.exchange(true, std::memory_order_acquire);
  }

  void unlock() noexcept {
    lock_.store(false, std::memory_order_release);
  }
};
```

### Семафор своими руками Semaphore C++ 11 Example
https://riptutorial.com/cplusplus/example/30142/semaphore-cplusplus-11
	
```
#include <mutex>
#include <condition_variable>
        
class Semaphore {
public:
    Semaphore (int count_ = 0)
    : count(count_) 
    {
    }
    
    inline void notify( int tid ) {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cout << "thread " << tid <<  " notify" << endl;
        //notify the waiting thread
        cv.notify_one();
    }
    inline void wait( int tid ) {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0) {
            cout << "thread " << tid << " wait" << endl;
            //wait on the mutex until notify is called
            cv.wait(lock);
            cout << "thread " << tid << " run" << endl;
        }
        count--;
    }
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};
```

### producer consumer c++11 своими руками

https://gist.github.com/iikuy/8115191

https://stackoverflow.com/questions/69453363/c11-simple-producer-consumer-multithreading
	
https://codereview.stackexchange.com/questions/84109/a-multi-threaded-producer-consumer-with-c11
	
- SMALL
```
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>
#include <atomic>

class Example {
  int c = 0;
  bool done = false;
  queue<int> goods;
  mutex mtx;
  condition_variable cond_var;
public:
  //Producer:
  void producer(int n) {
  	// Consumer can't touch goods
  	// until we finish. So no locks.
  	for (int i = 0; i < n; ++i) {
  		goods.push(i);
  		c++;
  	}
  	// Tell consumer he can 
  	unique_lock<mutex> lock(mtx);
  	done = true;
  	cond_var.notify_one();
  }
  void consumer() {
  	unique_lock<mutex> lock(mtx);
  	cond_var.wait(lock); //, [=]() {return done; });
  	// Producer finished updates.
  	// So we can simply loop over the goods and processes them
  	while (!goods.empty()) {
  		goods.pop();
  		c--;
  	}
  }
}; // Example

void main()
{
  int valc = 0;
  bool done = false;
  std::thread t1(&Example::producer, 4);
  std::thread t2(&Example::consumer);
  t1.join();
  t2.join();
  std::cout << "Net: " << valc << endl;
}
```

- BIG
```
#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <chrono>
#include <condition_variable>

class Buffer
{
public:
  std::mutex cout_mu;
public:
  void add(int num) {
    while (true) {
      std::unique_lock<std::mutex> locker(mu);
      cond.wait(locker, [this]() 
	{ return buffer_.size() < size_; }
      );
      buffer_.push_back(num);
      locker.unlock();
      cond.notify_all();
      return;
    }
  }
  int remove() {
    while (true)
    {
      std::unique_lock<std::mutex> locker(mu);
      cond.wait(locker, [this]() 
	{ return buffer_.size() > 0; }
      );
      int back = buffer_.back();
      buffer_.pop_back();
      locker.unlock();
      cond.notify_all();
      return back;
    }
  }
  Buffer() {}
private:
  // Add them as member variables here
  std::mutex mu;
  std::condition_variable cond;

  // Your normal variables here
  deque<int> buffer_;
  const unsigned int size_ = 10;
};

class Producer
{
public:
  Producer(Buffer& buffer)
  	: buffer_(buffer)
  {}
  void run() {
    while (true) {
      int num = std::rand() % 100;
      buffer_.add(num);
      {
      	std::unique_lock<std::mutex> locker(buffer_.cout_mu);
      	std::cout << "Produced: " << num << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
private:
  Buffer&  buffer_;
};

class Consumer
{
public:
  Consumer(Buffer& buffer)
  	: buffer_(buffer)
  {}
  void run() {
    while (true) {
      int num = buffer_.remove();
      {
      	std::unique_lock<std::mutex> locker(buffer_.cout_mu);
      	std::cout << "Consumed: " << num << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
private:
  Buffer&  buffer_;
};

int main()
{
  Buffer b;
  Producer p(b);
  Consumer c(b);

  std::thread producer_thread(&Producer::run, &p);
  std::thread consumer_thread(&Consumer::run, &c);

  producer_thread.join();
  consumer_thread.join();
  getchar();
  //return 0;
}
```
	
### threadpool / thread_pool / thread pool своими руками

https://www.youtube.com/watch?v=Ck5w-qNcKaw&t=1s
	
https://github.com/mtrebi/thread-pool

https://habr.com/ru/post/656515/

https://riptutorial.com/cplusplus/example/15806/create-a-simple-thread-pool
	
```
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

class thread_pool
{
public:
  explicit thread_pool(std::size_t thread_count = std::thread::hardware_concurrency())
  {
    if(!thread_count)
    	throw std::invalid_argument("bad thread count! must be non-zero!");

    m_threads.reserve(thread_count);

    for(auto i = 0; i < thread_count; ++i)
    {
    	m_threads.push_back(std::thread([this]()
    	{
          while(true)
          {
            work_item_ptr_t work{nullptr};
            {
            	std::unique_lock guard(m_queue_lock);
            	m_condition.wait(guard, [&]() { return !m_queue.empty(); });
            	work = std::move(m_queue.front());
            	m_queue.pop();
            }
            if(!work)
            {
            	break;
            }
            (*work)();
          }
    	}));
    }
  }

  ~thread_pool()
  {
    {
    	std::unique_lock guard(m_queue_lock);
    	for(auto& t : m_threads)
    		m_queue.push(work_item_ptr_t{nullptr});
    }
    for(auto& t : m_threads)
    	t.join();
  }

  thread_pool(const thread_pool&) = delete;
  thread_pool(thread_pool&&) = delete;
  thread_pool& operator = (const thread_pool&) = delete;
  thread_pool& operator = (thread_pool&&) = delete;

  using work_item_t = std::function<void(void)>;

  void do_work(work_item_t wi)
  {
    auto work_item = std::make_unique<work_item_t>(std::move(wi));
    {
    	std::unique_lock guard(m_queue_lock);
    	m_queue.push(std::move(work_item));
    }
    m_condition.notify_one();
  }

private:
  using work_item_ptr_t = std::unique_ptr<work_item_t>;
  using work_queue_t = std::queue<work_item_ptr_t>;

  work_queue_t m_queue;
  std::mutex m_queue_lock;
  std::condition_variable m_condition;

  using threads_t = std::vector<std::thread>;
  threads_t m_threads;
};

int main()
{
  using namespace std;
  srand(time(NULL));
  mutex cout_guard;  
  cout << "main thread ID: " << this_thread::get_id() << endl;  
  thread_pool tp;  
  for(auto i = 1; i <= 50; i++)
  {
    tp.do_work([&, i = i]() {
      {
  	unique_lock guard(cout_guard);
  	  cout << "doing work " << i << "..." << endl;
  	}
  	this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
    });
  }
}
```

### ВЗАИМНАЯ БЛОКИРОВКА
```
m1.lock
m2.lock
...
m2.unlock
m1.unlock
```

```
РЕШЕНИЕ:
m1.unlock
m2.unlock
```

### АТОМАРНАЯ ОПЕРАЦИЯ
АТОМАРНАЯ ОПЕРАЦИЯ - операция, которая не может быть прервана во время исполнения
						     
### ПРИМИТИВЫ СИНХРОНИЗАЦИИ
- http://phg.su/basis2/X610.HTM						     
- https://acm.bsu.by/wiki/Unix2019b/%D0%92%D0%BD%D1%83%D1%82%D1%80%D0%B5%D0%BD%D0%BD%D1%8F%D1%8F_%D1%80%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F_%D0%BF%D0%BE%D1%82%D0%BE%D0%BA%D0%BE%D0%B2
						     
```
  Синхронизация
   /         \
 spin        wait
```
- spin - поток выполнений (нить) остается активным и продолжает пытаться захватить ресурс
- wait - поток выполнения (нить) засыпает пока ресурс не будет освобожден

- mutex - Мьютекс- бинарный семафор, имеющий два состояния: свободный и заблокированный
- mutex - ◦ Именованные мьютексы – могут использоваться для межпроцессной синхронизации ◦ Неименованные мьютексы – могут использоваться только для межпоточной синхронизации
- criticall-section - Обычно критические секции используются только для межпотоковой синхронизации
- FUTEX - Fast Userspace Mutex (В ОС Linux - futex  В Windows – FAST_MUTEX)
- monitor - Только один поток может войти в монитор, Если монитор занят, то поток ожидает на входе в монитор
- condition-variable - очередь потоков - wait & signal
- spin-lock - ◦ Опрос обычной операцией ◦ Захват - атомарной
- spin-lock с очередью - ◦ Атомарная операция добавления номера потока в очередь ◦ Гарантируют отсутствие ресурсного голодания
- readers-writes lock (RWLock) – для ассиметричной схемы доступа к ресурсу


### Разница между Критической секцией и Мьютексом / Чем Критическая секция лучше Мьютекса
https://studizba.com/lectures/informatika-i-programmirovanie/lekcii-po-raspredelennym-vychisleniyam/4438-myuteks-semafor-kriticheskie-sekcii.html

1) Критическая секция - работает для синхронизации ПОТОКОВ, в рамках одного Процесса. А Мьютексом можно синхронизировать как Потоки так и Процессы (Но для процессов Мьютекс должен быть ИМЕНОВАНЫЙ) 
2) Критическая секция - объект ПОЛЬЗОВАТЕЛЬСКОГО режима, А Мьютекс - Мьютекс это объект ЯДРА
3) Критическая секция - не вызывает переключение контекста из ПОЛЬЗОВАТЕЛЬСКОГО режима в режим Ядра, А Мьютекс вызывает переключение контекста
4) 
						     
- ОБЩЕЕ: Критическая Секция - может использоваться только ОДНИМ ПОТОКОМ в данный момент времени
- ОБЩЕЕ: Мьютекс - Только ОДИН ПОТОК владеет этим объектом в любой момент времени может ("взаимное исключение" (mutex, mutual exclusion))
- ОБЩЕЕ: Мьютекс - Такой семафор, который в любой данный момент времени может удерживаться только ОДНИМ процессом или потоком (такой у которого счётчик может быть либо 0, либо 1, и НЕ БОЛЬШЕ)

РАЗЛИЧИЯ: Критическая секция не вызывает переключения контекста из Пользовательского режима в Режим Ядра, как это делает мьютекс.

- Критическая секция - Крит секция это объект ПОЛЬЗОВАТЕЛЬСКОГО режима.
- Мьютекс - Мьютекс это объект ЯДРА.

- Критическая секция - у неё нет HANDLE, дескриптора в ОС, чтобы обратиться по нему отовсюду к ней.
- Мьютекс - Мьютекс это объект ЯДРА, имеющий в рамках процесса HANDLE, но как только мы дали мьютексу ИМЯ, через имя объекта мы можем так же получить и его HANDLE в другом процессе (ещё можно получить его хэндл в другом процессе через DuplicateHandle).

- Критическая секция - синхронизирует Только ПОТОКИ, ибо доступна в пределах ОДНОГО ПРОЦЕССА.
- Мьютекс - синхронизирует и ПОТОКИ и ПРОЦЕССЫ. Для процессов ИМЕНОВАННЫЙ МЬЮТЕКС, для потоков можно использовать именованный или неименованный мьютекс.

- Критическая секция - работает на блокировке - нужна для синхронизации между ПОТОКАМИ, относительно небольшого участка кода, ибо основана на spinLock() схеме взаимно-исключающей синхронизации при разграничении доступа к общим ресурсам. Трудно предположить что-нибудь о порядке, в котором потоки будут получать доступ к ресурсу, можно сказать лишь, что система будет "справедлива" ко всем потокам.
- Мьютекс - работает на "сигналах" / "сигнальное / несигнальное состояние" - мьютекс захватывается потоком, поток делает своё дело, освобождает мьютекс, а остальные потоки, ожидают, освобождения объекта мьютекса, чтобы провести свои манипуляции с общим ресурсом

Особенности: Каждая операция с Мьютексом - захват, освобождение - требует переключение конткестного режима из пользовательского режима в контекст ядра.

Тоесть тратися время на сохраненеие регистров потока пользовательского режима, что-то свопается, что-то сохраняется. ЭТО ДОРОГО. У КРИТ.СЕКЦИИ - всё находится и происходит в рамках пользовательского процесса / режима, тоесть в ядро перехода напрямую не происходит. 
- ОСОБЕННОСТЬ: - Так как Мьютекс объект ядра, а значит доступен всем процессам, что означает, что шарить мьютекс между процессами можно по имени.

А Критическая Секция доступна в рамках ТОЛЬКО ОДНОГО Процесса. (Мьютекс безымянный синхронизирует потоки, а для синхронизации процессов можно использовать именованый мьютекс) 
- ОГОВОРКА: Критическая Секция - должна ограничивать сравнительно небольшой участок кода. В случае, когда секция которую мы залочили уж очень большая, то произойдёт следующее: Множество потоков пытаются в неё войти, а кто-то один её залочил и потоки ждут ДОЛГО, тогда для этой Крит.Секции создастся завязанный (ассоциированный) на эту критическую секцию объект ядра, типа Мьютекс - Который ЗАХВАТЫВАЕТСЯ тогда, когда уровень критического ожидания входа в Крит.Секцию является Достаточно большим.
(??? СТАС) Кто определяет, на сколько Долгий интервал ожидания и когда будет задействован "ассоциированный " Мьютекс
	
	
## Проблемы многопоточности

### 1) гонки потоков
— задержки(delay) / Слипы / таймауты (плохо, но на крайняк можно)
— использовать атомики (атомарные переменные и атомарные операции)

### Сколько нужно мьютексов, чтобы случился дедлок?
Ответ: МИНИМУМ ДВА

http://www.cyberforum.ru/cpp-beginners/thread921902.html
	
### 2) Дедлоки (avoiding circular wait)
- решается мьютексами
    - mutex - базовый мьютекс - mutex.lock() | mutex.unlock()
    - timed_mutex - мьютекс с тайм-аутом 
    - recursive_mutex - мьютекс, который может быть заблокирован рекурсивно из того же потока 
    - recursive_timed_mutex - мьютекс с таймаутом, который может быть заблокирован рекурсивно из того же потока 
    - shared_mutex - provides shared mutual exclusion facility 
    - shared_timed_mutex - provides shared mutual exclusion facility 
- порядком обращения к месту, где случается деллок (правильный порядок захвата мьютексов и их отпускания)
- std::lock_guard, std::unique_lock, std::try_to_lock, std::lock на мьютексах
    - lock_guard - реализует обёртку владения мьютексом строго в области видимости 
    - unique_lock - реализует подвижного мьютекс собственности обертку 
- condition_variable - решается условными переменными 
    - notify_all_at_thread_exit
- семафоры как частный случай мьютекса

- А так же для асинхронных уже не проблем а просто решений в 11 стандарте так же есть:
- packaged_task()
- future - future_error
- promise - future_error
- async
- launch

И ЕЩЁ общение между потоками / синхронизация потоков:
- События - std::condition_varianle
- Мьютекс - std::mutex
- Семафоры
- Критические секции (аналог в std std::lock_guard<std::recursive_mutex> locker(m);)
- SpinLock 
- Атомарные операции 

	defer_lock_t  -  try_to_lock_t  -  adopt_lock_t 
	
===================================================================

### explicit
explicit - когда мы хотим запретить НЕЯВНОЕ преобразование типов для параметра конструктора

### Этапы сборки
Этапы сборки: - проект в исполныемый файл
- директивы препроцессора все убираются, на их места ставтися всё что нужно, весь код формируется в один большой кусок кода
- этап компиляции - компиляция CPP-фапйлов файлов в объектные файлы
- линковка - нужна для сопряжения / совмещения в один общий исполняемый файл, для объединения нескольких объектных файлов в один исполняемый

Почему аргументы макросов нужно брать в скобки - чтобы по ходу использования макроса

Как headers - 

### ПОЛИМОРФИЗМ
1. Динамический - на этапе выполнения 
- механизм виртуальных функций (оверрайдим методы в классах наследниках и получаем по итогу разное поведение)
2. Статический: - на этапе компиляции
- 2.1 Шаблонные методы считаются стат.полим.
- 2.2 Разная сигнатура (2 метода с одинаковыми именами имеют разную сигнатуру по параметрам)

### Что произойдёт, если:
```
delete nullptr;
delete NULL; (макрос)
delete 0;
```
- НИЧЕГО не произойдёт.
- nullptr КАСТИТСЯ к bool - значению, и не кастится к арифметическому значению.
- только в этом плане nullptr это более безопасное зануление указателя, чем приравнивание такового к числу ноль / нулевому адресу

### Виртуальный деструктор - A базовый, B - наследник от А
```
class A
class B : public A
B b;         // A() // B() // ~B() // ~A()
A* a = new B; 
delete a;    // A() // B() //         ~A()
=>
virtual ~A()
delete a;    // A() // B() // ~B() // ~A()
```
- деструткор вызовется только для А, и не вызовется для B
- Утечка памяти, чтобы она не происходила, поэтому деструктор А делаем вритуальным, чтобы вызывался деструктор для ~B, и потом для ~A 

### Как вызовутся конструкторы классов, полей, наследников?
```
struct C : ABase, BBase
{
  BField b;
  AFirld a;
}
```
- конструкторы ABase и BBase вызовутся ранее конструкторов полей "а" и "b"
- конструктор вызовется у полей b - он раньше объявлен, a - он позже;
- сначала деструкторы Сам класс, далее сначала поле a, потом поле b и только потом родительсвие опять же в обратном порядке ~BBase ~ABase

### Можно ли бросать Исключение в КОНструкторе / Исключения в КОНструкторе
- Exception в конструкторе, он обработан в программе, но не обработан в рамках конструтора
могут произойти утечки, ибо для неких полей, которые являются указателями, если для них выделится память, 
то после исключения для них могут не вызваться из delete - в случае, если new - были в конструкторе, а delete были в деструкторе.
Но так как конструктор до конца не отработал, и деструктор не будет соотвественно для класса вызван.
ЗАЩИТА и РЕШЕНИЕ: УМНЫЕ УКАЗАТЕЛИ - даже в таких случаях деструкторы будут вызваны для созданных объектов.
```
struct A {
  A() {
    pb = new B; // pb - утечёт
    trow 1;
  }
  ~A() {
    delete pb; 
  }
  B *pb;
}; // Для данного класса т.к. есть указатель
// потребуется дописать и констр.копии и оператор=
// и по хорошему перемещающие-констр+операот=
// чтобы дописать всю доп работу с указателем B *pb

void test () {
  try { A a; }
  catch(...) {
  // сам pb занулится, а вот память под объект new B, останется в памяти
  }
}
```
	
### Можно ли бросать Исключение в ДЕСтрукторе / Исключения в ДЕСтрукторе
```
struct A {
  ~A() // можем добавить noexcept(false) и тогда можно бросать исключение
  { trow 1; }
};
try{
  A a;
}
catch(...) { }
// Программа упадёт: // 1. Все деструкторы 
// по умолчанию помечены как noexcept, 
// допишем к деструткору noexcept(false) 
// и тогда можно бросать исключение 
// и падения программы не будет
```

### Опасно ли бросать исключение в ДЕСтрукторе / Исключения в ДЕСтрукторе
- НЕЛЬЗЯ - это вызовет terminate (ибо деструкторы неявно помечаются как noexcept)
```
struct A {
  ~A() // можем добавить noexcept(false) и тогда можно бросать исключение
  { trow 1; }
};
try{
  A a;     // 2. сработает деструктор и выбросится второе исключение и это Необрабатываемая корректно ситуация, программа упадёт
  throw 2; // 1. Раскрутка стека пошла, начинают вызываться детсрукторы всех локальный переменных в данной области видимости
}
catch(...) { }

// сначала throw 1;   // 1. Раскрутка стека пошла, начинают вызываться детсрукторы всех локальный переменных в данной области видимости
// потом A a; -> ~A() // 2. сработает деструктор и выбросится второе исключение, по стандарту, когда на этапе размотки стека происходит ещё одно исключение, то вызовется команда std::terminate(), и программа упадёт
```

### Как мы можем Просигнализировать, что Сигнализируем, что Всё прошло хорошо/плохо в С++:
- optional in c++17
- errno - globavariable
- assert()
- return value
- exceptions (throw)

### POD - plain old data тип
```
class CD {
  CD() = default;
};
class CD1 {
  CD1() { }
};

CD  - может расцениваться как POD - plain old data тип
CD1 - НЕ может расцениваться как POD - plain old data тип
```

	
### ССЫЛКИ И УКАЗАТЕЛИ

- УКАЗАТЕЛЬ — переменная, в которой хранится адрес на какой-то объект.
- СЫЛКА — это указатель, с которым можно работать, как с обычной переменной.
```
ПУТАНИЦА И ЕЁ СУТЬ:
  Есть "&" - ССЫЛКА    - хранит указатель на объект 
// Есть Оператор "&" - ВЗЯТИЕ АДРЕСА переменной по которому она расположена

И есть "*" - УКАЗАТЕЛЬ - хранит адрес на объект    
// Есть Оператор "*" - РАЗЫМЕНОВАНИЕ Указателя т.е. взятия 
   значения по адресу который хранится в указателе
   
CУТЬ - для одних и тех же вещей ввели одинаковые символы.
```

```
int x;
int *y; // Объявили указатель  у  
y = &x; // От любой переменной можно взять адрес при помощи операции взятия адреса "&". Эта операция возвращает указатель
int z = *y; // Указатель можно разыменовать при помощи операции разыменовывания "*". Это операция возвращает тот объект, на который указывает указатель

int x;
int &y; // просто так объявить ссылку НЕЛЬЗЯ, нужно сразу же её проинициализировать, т.е. присвоить ей тут же значение (к чему-то приравнять)
int &y = x; // проинициализировали ссылку  у
int z = y;
```

### [Q] КАКОВ РАЗМЕР УКАЗАТЕЛЯ ? Т.е. int* n = new (5); sizeof(n)
В зависимости от платформы: на 32-бит машине 4 Байта. На 64-битной машине 8 байт.

### [Q] Каков размер ССЫЛКИ ?
Такой же как и размер указателя. В зависимости от платформы: на 32-бит машине 4 Байта. На 64-битной машине 8 байт.

### [Q] Разница между Указателем и ссылкой ?

Принципиальных отличий два 0), 1), 2) :

0) ссылка не может быть равна NULL. Указатель может.
1) ссылка, в отличии от указателя, не может быть неинициализированной;
2) ссылка не может быть изменена после инициализации. ТОЛЬКО Значение ПО ССЫЛКЕ Ссылка это как бы неизменяемый указатель.
3) Нельзя объявить массив ссылок.
4) У ссылки нет адреса.
5) Существует арифметика указателей, но нет арифметики ссылок.
6) Не может быть константной ссылки, только ссылка на константу. А констатный указатель может быть.

### [Q] Плюсы и минусы использования того и другого :
```
- Ссылки лучше использовать когда нежелательно
или не планируется изменение связи ссылка → объект;
- Указатель лучше использовать, когда возможны следующие моменты в течении жизни ссылки :
  -- ссылка не указывает ни на какой объект;
  -- ссылка указывает на разные объекты в течении своего времени жизни.

ПЛЮСЫ И МИНУСЫ:
Ссылка - (+)Всегда чем-то заполненный объект. 
(+)Живой объект, который не надо проверять на 
равенство нулю, или что она "освобождена" как память в указателе.
Указатель - (+)можно не проинициализировать, 
(+)можно приравнять к нулю, 
(+)в течение жизни может указывать на разные объекты, 
(-)НО может ссылаться на освобождённую память 
после применения к нему "delete".
```
### [Q] Почему в Си++ нельзя организовать полноценную сборку мусора ?
```
ОТВЕТ:
Из-за УКАЗАТЕЛЕЙ. И из-за адресной арифметики 
то есть арифметических операций с указателями.
Мы выделили область памяти, после произошло 
какое-нибудь смещение, а указатель нужно передать 
дальше в функцию сборщика, и собрать после смещений 
или тех операций, что происходили с указателем, 
будет невозможно, ибо сборщик всех операций 
учесть не сможет.
```

### [Q]  Разница между оператором присваивания и копирующим конструктором +их реализации ? 

1) Копирующий конструктор a(b); 
- инициализирует ранее неинициализированный (не существующий) объект 
из данных другого объекта (в момент создания объекта он сразу же инициализирует память.)
```
// A copy constructor is used to initialize a previously uninitialized object 
from some other object's data.

Реализация:
A(const A& rhs) : data_(rhs.data_) {}
```

2) Оператор присваивания a = b;  
- заменяет данные в ранее проинициализированном (созданном) объекте,
данными из другого объекта. (сначала создаёт объект, и потом 
перезатирает то, что он проинициализировал новым значением)
```
// An assignment operator is used to replace the data 
of a previously initialized object with some other object's data.

Реализация:
A& operator=(const A& rhs) { data_ = rhs.data_; return *this; }

ПРИМЕР вызова Копирующего конструктора:
Example for copy constructor :
Base obj1(5);
Base obj2 = obj1; //calls copy constructor 
                  // (ИБО новый объект obj2 
		  // только создался +не был ранее
		  // проинициализирован)
ПРИМЕР вызова Оператора присваивания: Example for assignment operator:
Base obj1(5); //calls Base class constructor
Base obj2; //calls Base class default constructor // вызывается конструктор по умолчанию
obj2 = obj1; //calls assignment operator (ИБО оба объекта уже были созданы ранее(проинициализированны)
```


### Что будет выведено на экран printf(“%d”, (int*)5 + 3)? // ответ 5+(3*sizeof(int))
Смещаемся относительно адреса "5" на 3 позиции типа int, а размер int = 4 байта => 5 + (3 * 4) = 5 + 12 = 17

### Какой тип по умолчанию присваивается числу с плавающей точкой 2.0?
В случае если тип не указан явно, присваивается double.


### Что вернёт операция sizeof для login, password, object?
```
char* password = "pass12";
char login[] = "ivanov_oleg";
class object; sizeof(object);
```
- Для password вернёт 4 или 8 в зависимости от платформы (для x86 вернёт 4, а для x64 вернёт 8) 
    - ИБО password есть указатель.
- Для login вернёт 12
    - ИБО массив символов инициализируется строкой, в которой на первый взгляд 11 символов, НО в конце строки есть "/0" так называемый "закрывающий ноль" он не видим. (все строки в Сях инициализируются как нультерминируемые строки)
- Для object вернёт 1
    - ИБО в С++ размер всех объектов больше 0. Размер любого объекта (даже пустого класса) по стандарту не может быть меньше 1го байта. Это реализовано затем, чтобы адрес каждого объекта был отличным от адреса любого другого объекта. Поэтому в нашем размер объекта пустого класса равен 1-му байту.

	
### Размер пустого класса
```
struct A{};
int sz = sizeof(A);
```
Согласно стандарту:
- В С++ размер всех объектов больше 0. Размер любого объекта (даже пустого класса) по стандарту не может быть меньше 1го байта. Это реализовано затем, чтобы адрес каждого объекта был отличным от адреса любого другого объекта. Поэтому в нашем размер объекта пустого класса равен 1-му байту.
- Размер любого объекта программы на С++ по стандарту не может быть меньше 1 байта (точнее одного идентифицируемого на данной платформе элемента памяти). Это связано с тем, что никакие два разных объекта не могут иметь одинаковые адреса. Если размер объекта был бы 0, то в массиве таких объектов все элементы имели бы одинаковый адрес, а это недопустимо.

### Размер наследника пустого класса
```
struct A{};
struct B : A {};
sizeof(A) = 1, sizeof(B) = 1
// ОТВЕТ Empty Base class optimization - если базовый пустой и пустой наследник, то оба будут весить 1 байт
```

### Размер пустого класса и указатель на this
- Указатель **this** не является членом класса!
- Компилятор добавляет этот указатель НЕЯВНО первым параметром ко всем вызовам методов (скрытым параметром)

```
struct Object { void method(int i); };
```
Компилятор превартит примерно вот во что:
```
void Object::method(Object* this, int i);
// см. на первый добавленный компилятором первый параметр
```
ТО ЕСТЬ ДЛЯ:
```	
Object object;
object->method(5);
// Компилятор превратит в что-то типа:

Object object;
Object::method(&object, 5);
// Если смотреть на дизассемблированный код, то
// Можно будет увидеть скрытый добавленный первый параметр
```
	
**Правило трёх 3-ёх**: Если у нас есть нетривиальный конструктор, либо оператор присванивания либо констр.копии, 
и ЕСЛИ нам нужно реализовать один из этих конструкторов, то нам нужно реализовать сразу ВСЕ.

**Правило пяти 5-ти**: для МУВ-семантики: Если у нас есть нетривиальный конструктор, либо оператор присванивания либо констр.копии / оператор перемещения, констр.перемещения, 
и ЕСЛИ нам нужно реализовать один из этих конструкторов, то нам нужно реализовать сразу ВСЕ.


### placement new - Как можно создать объект класса по определённому адресу в памяти ? по конкретному адресу в памяти ?
Использовать оператор placement new

ЗАЧЕМ:
- Однин и тот же участок памяти чтобы брать и с ним работать. Чтобы лишний раз не "дёргать" кучу.
- Если у нас памяти мало, или есть выделенный участок
- Если мы хотим облегчить нашу работу не обращаться к ОС для удаления или для создания, просто можем брать и создавать объкты и удалять их по одним и тем же указателям

### [Q] Как зпретить копирование объекта класса / Как защитить объект от копирования?
РАНЬШЕ - Ответ: Сделать private конструктор копирования и оператор присваивания "="

```
struct NonCopyable
{
  NonCopyable() { }
  private:
  NonCopyable(const NonCopyable&)
  NonCopyable& operator=(const NonCopyable&)
};
void main() {
 NonCopyable a; 
 NonCopyable b = a; // error C2248: 'NonCopyable::NonCopyable' : cannot access private member
 a = b; // error C2248: 'NonCopyable::operator =' : cannot access private member
}
```

ТЕПЕРЬ C++11:
```
struct NonCopyable
{
  NonCopyable() = default; // конструктор по умолчанию
  NonCopyable(const NonCopyable&) = delete; // удалит метод из класса
  NonCopyable& operator=(const NonCopyable&) = delete; // удалит метод из класса
};
void main() {
 NonCopyable a; 
 NonCopyable b = a; // error C2248: function N(c N&) cannot be referenced -- it is a deleted function
 a = b; // error C2248: 'NonCopyable::operator =' cannot be referenced -- it is a deleted function
}
``` 

подробнее тут - https://habrahabr.ru/company/abbyy/blog/142595/
	
### Как запретить наследование от класса / Как защитить класс от возможности наследования / от возможности наследоваться от него
- Заприватить конструкторы 1. обычный 2. костр.КОПИИ 3. оператор присваивания
- final - ТРЕБУЕТ УТОЧНЕНИЯ !!! - и нельзя будет наследоваться от этого класса ... 
```
С С++11
class Foo final {};
class Bar: public Foo {}; // Fails to compile as Foo is marked as final
```

### Разница между uniqie_ptr и shared_ptr СЧЁТЧИК ССЫЛОК
- СЧЁТЧИК ССЫЛОК (указатель), то есть в **unique_ptr - указатель один**, на сам объект, А у **shared_ptr - указателя два**, один указатель на объект и второй указатель это и есть **счётчик ссылок**
- shared_ptr можно копироват, А unique_ptr - указатель нельзя копировать, но можно передавать владение им с помощью std::move

### Умные указатели / smart pointers / smart_pointers СЧЁТЧИК ССЫЛОК
- uniqie_ptr - умный указатель **БЕЗ счётчика** ссылок, он сам уникально владеет объектом (когда наоборот хотим не шарить объект а гарантировать монопольное использование) 
```
template<typename T>
class my_unique_ptr
{
  T* p_data = nullptr;
}
```
- shared_ptr - умный указатель **С СЧЁТЧИКОМ ССЫЛОК** на объект (объект жив пока счётчик ссылок не равен нулю)
```
template<typename T>
class my_shared_ptr
{
  T* p_data = nullptr;
  int *ref_count = nullptr;
}
```
- weak_ptr - он хранит указатель так же как и shared_ptr, но он не считает ссылки на объекты, нужен для решения проблемы циклических/колцевых ссылок (2 шаред поинтера не могут удалиться по итогу)

### shared_ptr() и make_shared()
```
void func() {
  func1(std::shared_ptr<A>(new A), func_ex());
}
```

1) **shared_ptr** - состоит из ДВУХ вещей: 1. Сами данные и 2. Контрольный блок с счётчиком ссылок
- Память под shared_ptr и указатель на сам объект выделяется за два шага ВЫДЕЛЕНИЯ ПАМЯТИ и фрагментировано, то есть в разных местах.
- В случае с shared_ptr(new type) память для счетчиков аллоцируется отдельно от объекта. И поэтому, располагаться эти счётчики могут где угодно (почти наверняка - далеко от расположения объекта)
- МНУС (*) Если вы не используете функцию make_shared, то придется использовать явное выражение **new** для создания объекта, прежде чем передавать его в конструктор shared_ptr, а **new** может бросить исключение (std::bad_alloc)

2) make_shared() выделение КАК-ПРАВИЛО (typically performs only one allocation) произойдёт за одно выделение памяти, согласно стандарту.
- 1. инициализацию shared_pointer-a и 2. самих данных на которые указывать будет поинтер ЗА ОДНО ВЫДЕЛЕНИЕ ПАМЯТИ.
- В случае с make_shared счетчики объектов всегда располагаются рядышком с самим объектом. Потому что расположены в том же самом блоке данных, в котором расположен сам объект.
- ПЛЮС (*) Используйте функцию make_shared для создания указателя shared_ptr. Функция make_shared безопасна в отношении исключений. Для выделения памяти под блок управления и ресурс используется один вызов, что снижает накладные расходы. 

3) СУТЬ ПРОБЛЕМЫ
Создаём std::shared_ptr из свежесозданного сырого указателя может приводить к утечке памяти:
```
func ( std::shared_ptr<Bar>(new Bar), foo() );
```
- Для вычисления аргументов функции func(...) необходимо вызвать: 1. new Bar; 2. конструктор std::shared_ptr; 3. foo().
- Компилятор может **перемешать порядок выполнения эти трёх операций**, ибо порядок обработки параметров функции стандартом не определён, итог:
1. new Bar; 2. foo(); 3. конструктор std::shared_ptr. Если при этом **в foo() возникнет исключение** — получаем утечку экземпляра Bar.
- Ни один из следующих примеров кода не содержит потенциальную утечку (но мы ещё вернёмся к этому вопросу):
```
auto bar = std::shared_ptr<Bar>(new Bar);

auto bar = std::shared_ptr<Bar>(new Bar);
process(bar, foo());

process(std::shared_ptr<Bar>(new Bar));
```

4) РЕШЕНИЕ std::make_shared бесполезный

Начиная с c++17 утечка памяти в том хитром редком примере, ради которого в STL был добавлен std::make_shared, уже невозможна.

### Q: Объясните принцип полиморфизма. 
Полиморфизм - возможность работать с разными типами одинаковым образом. Статический - перегрузка функций, шаблоны. Динамический - виртуальные функции.

//
// ПОМНИМ ЗАЧЕМ ВООБЩЕ НУЖНО СЛОВО friend
//

friend - функции - это функции, которые не являются членами класса, однако имеют доступ к его закрытым членам - переменным и функциям, которые имеют спецификатор 
private.

friend - классы - Все методы класса A будут иметь доступ к скрытым членам класса B.
```
class A { };
class B
{ friend class A; };
```

```
class B {
  public: 
  virtual ~B() = default;
};
class Child : protected B
{
  friend void f(); // этой строки не было. БОЛЕЕ ТОГО именно добавив friend код скомпилируется
};

void f() {
  B* b = new Child;
}
```


# explicit - зачем нужно
Чтобы не позволить вызвать конструктор от одного аргумента неявно

### [Q] 48. Как запретить неявное преобразование типа, выполняемое конструктором инициализации?
Ответ: Используя ключевое слово explicit (ПРИМЕР см. 2 вопросами ниже)

### [Q] 59. Для чего объявлять конструктор с ключевым словом explicit?
Таким образом происходит подавление неявное приведение типа. Такой конструктор можно вызывать только явно. (ПРИМЕР см. вопросом ниже)

### [Q] 50. Для чего служит ключевое слово explicit?
Это модификатор для запрета неявного преобразования типов объектов аргументов конструктора.

```
#include <iostream>
class B
{
  int m_var;
  public:
  B() { }
  B(int var) : m_var(var) { }
  void print() { std::cout << "print!" << std::endl}
}

void fun(B b)
{ b.ptint(); }

int main()
{
  B obj1(10);  // Обычный вызов конструктора
  B obj2 = 10; // implicit call 
               // НЕЯВНОЕ ПРЕОБРАЗОВАНИЕ и вызов Конструктора с параметром
               // На самом деле вызовется  obj2( 10 );
  fun(obj1); // Обычный вызов конструктора
  fun(30);   // implicit call 
             // НЕЯВНОЕ ПРЕОБРАЗОВАНИЕ и вызов Конструктора с параметром
             // На самом деле вызовется  fun(B( 30 );
}
```
ЕСЛИ в классе перепишем так: **explicit B(int var) : m_var(var) { }**

ERROR 1 будет: B obj2 = 10; // convertion from int to NON Scalar type

ERROR 2 будет:   fun(30);   // can not convert from int to B


### [Q] Назовите четыре варианта приведения типов?

1) const_cast – аннулирует const и volatile -  убирает константность объекта. Никакие другие приведения в С++ стиле не позволяют такое сделать.
2) static_cast – иерархическое преобразование с проверкой на этапе компиляции - явное приведение типов, например, и в основном 1) для встроенных типов (Int к double), то будут использованы встроенные в C++ правила их приведения. 2) Если это типы, определенные программистом, то будут использованы правила приведения, определенные программистом.
static_cast - ещё варианты: приведение неконстантных объектов к константным. между указателями корректен, только если один из указателей - это указатель на void или если это приведение между объектами классов, указателей на базовый класс к указателю на производный.
3) dynamic_cast — проверяется на этапе выполнения RTTI -  применяется для выполнения безопасного понижающего приведения. Позволяет определить, принадлежит ли объект данного типа некоторой иерархии наследования. Единственное приведение, которое не может быть выполнено с использованием старого синтаксиса в Си-стиле. Требует определённых временных затрат.
4) reinterpret_cast — несвязанное преобразование с проверкой на этапе компиляции -  Обычно используется, чтобы привести указатель к указателю, указатель к целому, целое к указателю. Умеет также работать со ссылками. Результат может быть некорректным, никаких проверок не делается. Считается, что вы лучше компилятора знаете как на самом деле обстоят дела, а он тихо подчиняется. Не может быть приведено одно значение к другому значению. 

Предназначен для низкоуровневых приведений, которые порождают зависимые от реализации (непереносимые) результаты, например приведение указателя к int. Вне низкоуровневого кода такое приведение должно использоваться редко.


### 10.2 Почему стоит использовать преобразования типов в C++ стиле?
Потому что оно более безопасное. Си-стиль позволяет привести почти всё что угодно ко всему, без проверок от компилятора! C++-стиль приведения более заметен, чем Си-стиль.


### 11.1 Чем отличается dynamic_cast от других вариантов приведения типов?
Безопасное приведение по иерархии наследования, в том числе и для виртуального наследования.

dynamic_cast<derv_class *>(base_class_ptr_expr)

Используется RTTI (Runtime Type Information), чтобы привести один указатель на объект класса к другому указателю на объект класса. 
Классы должны быть полиморфными, то есть в базовом классе должна быть хотя бы одна виртуальная функция. Если эти условие не соблюдено, ошибка возникнет на этапе компиляции. Если приведение невозможно, то об этом станет ясно только на этапе выполнения программы и будет возвращен NULL.

	
# Различие между С и С++ / между C и C++ / Разница между С и С++ / между C и C++

- Си (структурное прогр.) | В С++ (Объектно ориентированное прогр.)
- Си (отсуствует полиморфизм) | В С++ (Полиморфизм работает через виртуальные функции)
- Си (можно вызывать main() из других фций) | В С++ (нельзя вызывать main() из других фций)
- В С код программы хранится в файлах с раширением ".c" | В С++ код программы хранится в файлах с раширением ".cpp"
- В С для работы с консолью используются scanf() и printf() | В С++  std::cout и std::cin
- default-аргумент (В Си у фций НЕТ аргументов по умолчанию | В С++ у фций МОЖНО использовать аргументы по умолчанию)
- Объекты (В Си нет модификаторов доступа private, protected, есть структуры в которых всё public | В C++ структуры могут содержать в себе все три мдификатора доступа public, private, protected, и в классах тоже)
- Структура / Класс (1. в структуре по умолчанию все поля публичные, в Классе по умолчанию все поля приватные. 2. В структуре по умолчанию публичноре наследование, в классе по умолчанию приватное наследование)
- Наследование (То, что в C++ - наследование, в C - это просто структура в структуре. При программировании в стиле C++ применяются такие красивые и звучные слова, как "класс Circle порожден от класса Point" или "класс Point наследуется от класса Circle и является производным от него". На практике все это словоблудие заключается в том, что структура Point - это первое поле структуры Circle.
При этом реальных усовершенствований два. ПЕРВОЕ - поля Point считаются так же и полями Circle, в результате доступ к ним записывается короче, чем в C. ВТОРОЕ - в обоих структурах можно иметь функции-методы, у которых имена совпадают с точностью до имени структуры. Например, Point::paint и Circle::paint . Следствие - не надо изобретать имена вроде Point_paint и Circle_paint, как это было в C,)
- new и delete (В Си пользоватлись malloc() и free | В С++ пояились new и delete) // Point *p =malloc(sizeof(Point)); и free(p);  Point *p = new Point; delete p;)
- Конструкторы и деструкторы (В Си заведя переменные их надо проинициализировать отдельно, автоматом конструктор не отработает и не поможет нам, и деструктора тоже нет, который очистит за нас содержимое перемиенных и указателей в структуре | В С++ конструктор вызывается автоматически, но инифилизацию можно написать сразу в него, Деструктор есть вызовется атоматом, деинициализирует сам содержимое POD элементарных типов, содержимое указатели праввда надо всё равно осводить в ручную)
- virtual и firiend - Виртуальные функции - (В Си отсуствует механизм виртуальных фций, поэтому механизм работы с таблицами виртуальных функций (через указатель на соотвествующую таблицу для структуры), если необходимо, то придётся реализовать "своими руками". | В C++ есть поддержка ключевого слова virtual и механизма виртуальных функций и работа с указателем на vtable, а так же есть и ключевое слово friend)
- Исключения (В Си НЕТ поддержки механизма обработки исключений | В С++ ЕСТЬ поддержки механизма обработки исключений)
- Перегруженные операторы (В Си НЕТ перегрузки операторов | В С++ ЕСТЬ перегрузка операторов)
- Ссылка (В Си НЕТ ссылок, но есть указатели | В С++ ЕСТЬ и ссылки и указатели)
- inline (В Си НЕТ поддержки inline фций | В С++ ЕСТЬ поддержки inline фций)
- namespace (В Си НЕТ поддержки ключевого слова namespace | В С++ ЕСТЬ поддержка namespace для непересекаемости имён объектов)
- template 

### new и delete
- Конструкции new и delete следует рассматривать как транзакции.
- **new** сначала выделяет блок динамической памяти подходящего размера, а потом конструирует в этой памяти элемент.
- **delete** сначала вызывает деструктор объекта, а потом возвращает динамическую память системе.
	
### Какие операторы НЕЛЬЗЯ перегружать ?
ПЕРЕГРУЖАТЬ НЕЛЬЗЯ
```
 :: (разрешение области видимости)
 . (выбор члена)
 .* (выбор члена через указатель на член)
 ?: тернарный оператор
 sizeof, typeid

У первых трех в правой части имя, а не значение.
У тернарного оператора аж 3 параметра, 
к тому же его возвращаемое значение является l-value.

Переопределять sizeof, typeid, думаю, просто нет смысла.
Для ВСТРОЕННЫХ ТИПОВ данных перегружать операции НЕЛЬЗЯ, потому что тип — это
набор данных и операций над ними. Изменяя операции, выполняемые над данными типа, 
мы меняем само определение типа. Также, НЕЛЬЗЯ изменить ПРИОРИТЕТ Операции и 
определить новую операцию.
```	


АССОЦ-КОНТЕЙНЕР:
- оператор меньше
- 

===================================================================

	
# WINDOWS OS / OS WINDOWS / WINDOWS QUESTIONS

	
### Типы ОБЪЕКТ ЯДРА
- маркеры доступа / access token
- события / event
- файлы / file
- проекции файлов / file mapping
- порты завершения ввода-вывода / I/O completion port
- задания / Job
- почтовые ящики / mailslot
- мъютексы / mutex
- каналы / pipe
- процессы / process
- семафоры / semaphore
- потоки / thread
- ожидаемые таймеры / waitable timer

### ОБЪЕКТ ЯДРА
(ОЯ) ОБЪЕКТ ЯДРА - Это Блок памяти внутри Ядра, выделенный под оперделённые задачи, которые мы просим выполнить ядро, или которые ядру нужно сделать самому
- HANDLE объекта (дескриптор из таблиц дескрипторов в процессах, "указывающих" на сами объекты ядра, которые находятся в области памяти ядра)
- имя объекта ядра
- Класс (ID) защиты объекта ядра
- счетчик количества пользователей объекта ядра
- обладают свойством наследования
- Создаются через спец.функции HANDLE CreateSemaphore / HANDLE CreateFile / 
- Закрываются обязательно через HANDLE объекта и спец.функцию BOOL CloseHandle( HANDLE hObject ); // принимает дескриптор объекта

### HANDLE
HANDLE - 32разрядный INT - Дескриптор какого-то объекта Ядра. У каждого процесса есть своя таблица Дескрипторов, в этих таблицах и хранятся хэндлы  "указывающие" на сами объекты ядра, которые находятся в области памяти ядра.

### Процесс от потока - разница ? (один есть основной поток)
1) Процесс - (!) ТО, что ИСПОЛНЯЕТ код внутри процесса и пользуется всеми ресурсами, которые предоставил потоку его процесс.
2) Поток - (!) ТО, что ХРАНИТ ДАННЫЕ и ВЫДЕЛЯЕТ ресурсы системы, для предоставления этих данных и ресурсов Потокам для исполнения кода программы. Память каждого потока изолирована от других потоков.
3) Процесс это контейнер для потоков.
4) Несколько потоков пользуются общим адреснымс пространством процесса

	
### ПРОЦЕСС / Процесс / ППроцесс / ппроцесс / Process
(!) ТО, что **ХРАНИТ ДАННЫЕ и ВЫДЕЛЯЕТ ресурсы системы**, для предоставления этих данных и ресурсов Потокам для исполнения кода программы. Память каждого потока изолирована от других потоков.

ПРОЦЕСС - **ЕДИНИЦА ХРАНЕНИЯ КОДА И (ПРЕДОСТАВЛЕНИЯ) РЕСУРСОВ ПРОГРАММЫ(Е)** 1. эксземпляр исполняемой программы со своим отдельно под неё выделенным виртуальным адресным пространстовм 2. некое состояние выполнения операции
- 0. Хранит в себе 1. PID - уникальный идентификатор процесса 2. список открытых дескрипторов (HANDLE) различных системных ресурсов 3. статистическую информацию.
- 1. собственное виртуальное адресное пространство (Адресное пространство - диапазон адресов виртуальной памяти, которым может пользоваться процесс)
- 2. исполняемый код / исполняемая программа
- 3. данные, потребляющие ресурсы ОС, то есть данные проецируемые на виртуальное адресное пространство
- 4. СТЕК программы
- 5. Имеет связанный набор РЕГИСТРОВ в которых хранятся состояния СЧЁТЧИКОВ (Пример: "Счётчик команд" - регистр с адресами следующей стоящей в очереди на выполнение команды)
	
### ПОТОК / Поток / ППоток / ппоток / Thread
(!) ТО, что **ИСПОЛНЯЕТ** код внутри процесса и пользуется всеми ресурсами, которые предоставил потоку его процесс.

ПОТОК - Поток (thread) – **ЕДИНИЦА ИСПОЛНЕНИЯ КОДА (ПРОГРАММЫ)** команд в ранках процесса. Поток не процесс! POSIX определяет набор интерфейсов (функций заголовочных файлов) для программирования потоков.
- Процесс в ОС может состоять из нескольких потоков, выполняющихся "параллельно"
- Существует как минимум один основной поток / главный поток в рамках выполняющегося процесса.
- Все потоки процесса выполняются в адресном пространстве процесса.
- Все потоки процесса пользуются ресурсами процесса.
- Он отвечает за ИСПОЛНЕНИЕ кода в адресном пространстве процесса
- Имеет набор регистров процессора, отражающих состояние процессора
- Имеет 2 стека: 1-ый Юзер-модный, а 2-ой Кернел-модный. Для исполнения в 1. юзер-мод (пользоват.) режиме, а 2ой для исполнения в 2. кернел-мод режиме (режиме ядра)
- Стек потока так же содержит параметры всех функций и локальные переменные для выполнения потоком кода 
- И так же содержит в себе TLS ThreadLocalStorage - локальная память потока, где переопределены, в частности, все глобальные константы, к-рые ф-ции CRT-библиотеки юзают. Например, такая как errno. Именно TLS - гарантия, что ф-ции CRT-библиотеки будут работать корректно.
- Поток завершает свою работу либо сам, либо принудительно, если завершает работу его Процесс-родитель.

### Createthread разница _beginthreadex

ТЕЗИС: Лучше использовать _beginthreadex, чем CreateThread().
	
- _beginthread - актуален, если используешь многопоточную версию crt. Иначе возникнут проблемы при использовании глобальных переменных и т.п., ибо в первой версии crt и понятия не было о многозадачности (см.книгу Джефри Рихтера), а “ex” – просто более широкий набор параметров, и всё. 
- _beginthread и _beginthreadex - функция библиотеки CRT (си рантайм либ), НЕ кроссплатформенна, но приспособленна для работы с С, т.е. она делает дополнительные манипуляции, что бы стандартные библиотеки корректно работатли (+ вызывает CreateThread()) .
- _beginthreadex - RunTime-функция: она запустится - выделит память в Thread Local Storage и будет ею пользоваться.
- Когда процесс будет завершать работу, то _beginthread(ex) - удалит так же ещё и вспомогательную информацию, что хранится в этих самых рантайм-бибоиотечных объектах.
- (!) Если объекты рантайм-библиотечные не удалить, то может произойти своеобразная утечка памяти, от чего ЗАЩИЩАЕТ _beginthreadex.

- CreateThread - чисто Win32Api'шная функция, просто создающая на уровне ядра объект потока.
- CreateThread  (не создаёт такого окружения для работы с рантаймовскими библами и объектами, как _beginthread(ex)) не запускается таким образом, чтобы создать рантайм-объекты и как следствие он не вызывает и деструкторов этих объектов. 
- (!) Если объекты рантайм-библиотечные не удалить, то может произойти своеобразная утечка памяти, от чего НЕ защитит CreateThread().

- СУТЬ: 
http://forum.vingrad.ru/forum/topic-47554.html

В общем если пишешь на С, то лучше пользоваться вторым.
Рихтер в статье CreateThread vs. _beginthread очень подробно описывает почему _beginthread предпочтительнее. Она создает **Thread Local Storage (TLS)**, где переопределены, в частности, все глобальные константы, к-рые ф-ции CRT-библиотеки юзают. Например, такая как errno. Именно TLS - гарантия, что ф-ции CRT-библиотеки будут работать корректно. 

The C runtime library was delivered in a UNIX context, in which there is no distinction between processes and threads. In the Windows context, many threads can be executing in a single address space. 

Microsoft has provided an alternative function to CreateThread, called _beginthreadex, to be used with the programs that use multiple threads at the same time they use the C runtime library. The problem occurs with any globally accessible variable used by this library ( there are several of them ). The Microsoft solution is to have the C runtime library provide a copy of each of these variables for each thread. Then, when a thread interacts with the runtime library, variables are shared only between the runtime code and the thread, not among all threads. The _beginthreadex function creates the copy for a thread in conjunction with an embedded call to CreateThread.

### СООБЩЕНИЯ в WINDOWS
- Внешние устройства (клава, мышь)
- Уведомления ПО о событиях в ОС от ОС
    - ПО_1 -- уведомить_ОС --> ОС -- сообщение_ПО --> ПО_2
    - ПО_1 --> ПО_1 (самому себе)
    - ПО_1 --> ПО_2 --> ПО_3
```
      Сообщения от
 ---- драйверов
 |    Мыши клавы
 |
 |__> Кладётся в 
 ---- очередь сообщений
 |
 |    Далее смотрим
 |__> С каким активным окном
 ---- связанно сообщение
 |
 |__> Сообщение
         \/
   т---------------т
   | Приложение    |
   |  _____V_____  |
   |  |  оконная | |
   |  |  функция | |
   |  |____v_____| |
   |  | Обработка| |
   |  | сообщения| |
   |  |__________| |
   |_______________|
   
```

### Оконная функция HRESULT CALLBACK WndProc(wnd, IntMsg. WPar, LPar)
Доступна для вызовов извне приложения только ОС

Функция окна - формирует сообщения, описывающие события м направляет его в нужную функцию окна.
- ф-ция анализирует сообщения и выполняет соотвествующие действия
- обрабатывает сообщения, распределяемые окну
- ОДНО Окно <=> ОДНА Оконная функция
- вызов в функции окна осуществляет только ОС, из main() её не вызвать
```
Окна:
- Родитель
  - потомок_1
  - потомок_2
  ...
  - потомок_N
```
Главынй родитель - Окно DESKTOP (дочерние окна без родителя)
	
```
  _______
 |над    |________________
 |потомок|   ПОДРОДИТЕЛЬ  |
 |_______|                | 
   |             _______  |
   |            |над    | |
   |            |потомок| |
   |            |_______| |
   |______________________|
	
```	
Зкрыли родителя, Закрылм всех потомков
	
### PostMessage SendMerssage
- SendMessage - Вызывает оконную функцию напрямую, БЕЗ постановки в очередь обработки сообщений
- PostMessage - ставится в очередь обработки сообщений внутри программы
- GetMessage - получить сообщение из очереди сообщений
- TranslateMessage - трансляция, например от клаиватуры
- DispatchMessage - перенаправление оконной функции (return результат обработки)
	
### FileMapping
```
|Файл|Проекция Файла|Окно проекции (View)|
```	
- CreateFileMapping 
- MapViewOfFile 
- lpMapAddress + iViewDelta
- UnmapViewOfFile
- CloseHandle
	
### Wait функции
- WaitForSingleObject
- WaitForMultipleObjects

### Синхронизация потоков
- Мьютексы (kernel-space и НЕименовынные для потоков / Именованные для процессов)
- Крит. секции (user-space + только для потоков)
- Condition Variable
- События
- Семафоры
- Атомик-переменные
- Атомарные операции

### Синхронизация процессов
- Именовынные Мьютексы для процессов (Неименованные для потоков)
- Файлы
- Пайпы = Канал (Именованный/Неименованный канал)
- Сокеты
- Сигналы
- Shared Memory / Разделяемая память
- Проецируемый в память файл (mmap (Linux) / FileMapping (Win))
- Обмен сообщениями (в частности Оконные сообщения)
- Очередь сообщений (Message queue)
- МэйлСлоты
- Семафор


Проецируемый в память файл (mmap)	Все ОС, совместимые со стандартом POSIX. При использовании временного файла возможно возникновение гонки. ОС Windows также предоставляет этот механизм, но посредством API, отличающегося от API, описанного в стандарте POSIX.
Очередь сообщений (Message queue)	Большинство ОС.
Почтовый ящик	Некоторые ОС.
	
### Оконная функция
http://lavimidj.ru/860/

- SendMessage (синхр.)
- PostMessage (асинхроння)
- GetMessage (while ((iGetOk = GetMessage(&msg, NULL, 0, 0 )) != 0) //цикл сообщений)
- TranslateMessage
- DispatchMessage
- PostQuitMessage
- WndProc - Обработка сообщений
	
### COM-объекты и QueryInterface
У COM технологии есть всего 2 отвественности:
- Запросить интерфейс
- вести подсчёт ссылок
	
```
	                      VTBL
	   COM-Component      ptr to Interface_1
Client --> VTBL-Pointer --->  ptr to Interface_2  
	   Obj. Data          ptr to Interface_N
```
	
### COM-объекты и QueryInterface
- Клиент использует QueryInterface-метод чтобы запросить интерфейс, содержащийся в COM-компоненте
```
COM-комп.1 | COM-комп.2
____       | ____
|   -- A   | |   -- X
|   -- B   | |   -- Y
|__ -- C   | |__ -- Z

A,B,...Y,Z - Интерфейсы
```
- у любого COM интерфейса/компонента/класса есть GUID
- COM компонент хочет воспользоваться интерфейсом, и вызывает AddRef()
- COM компоненту больше не нужен интерфейс - вызываем Release()
- COM компонент остаётся загруженный в память пока счётчик ссылок на него не станет равен нулю
- CLSID - используется в COM-библиотеках, чтобы загрузить СОМ-компонент

### Соглашения о вызовах _cdecl, _pascal, _stdcall / Windows
- _pascal - ПРЯМОЙ - параметры передаются на стек в ПРЯМОМ порядке
- _cdecl - ОБРАТНЫЙ - параметры передаются на стек в ОБРАТОМ порядке
- _stdcall / Windows - ОБРАТНЫЙ - параметры передаются на стек в ОБРАТОМ порядке + Вызванная продпрограмма освобождает стек!

### Статический импорт библиотек DLL
- Это когда мы загружаем библиотеку в память Процесса. 
- Для этого нам нужен lib-файл (библиотека импорта)
- Нам нужно знать имена вызываемый функци и какое в данной библиотеке функции библиотеки используется Соглашение о вызовах функций _cdecl, _pascal, _stdcall / Windows (см. выше)

**СУТЬ**: Фактически библиотека с её содержимым приростает к программе, загружаясь полностью в адресное пространство Программы, становится её частью

```
DLL       ПРОГА           DLL            ПРОГА
_______   ______  С   ______________     _______________
|ф-ции | |      | Т  | (фция_библы)-|---|->(фция_библы) |
|библы | |      | => |              |   | адресное      |
|      | |      | А  |              |   | пространоство |
|______| |______| Т  |______________|   |_______________|
```

### Динамический импорт библиотек DLL
- Для обращения к функциям библиотеки "подгружаем" каждую из них когда нам надо через спец. функции:
0) HMODULE GetModuleHandle ( LPCTSTR lpModuleName ) // Имя модуля (если грузим модуль)
1) HINSTANCE LoadLibrary ( LPCTSTR lpLibFileName ) // Имя файла DLL (если грузим DLL)
2) FARPROC GetProcAddress ( HMODULE hModule, LPCSTR lpProcName) // получить адрес функции  // указатель на модуль // имя функции
3) BOOL FreeLibrary( HMODULE hLibModule ); // закончили работу с DLL / модулем

**СУТЬ**: Мы каждый раз через чпец функции обращаемся к библиотеке и вызываем её функции, НО наш процесс не подгружает всю библиотеку в своё адресное пространство, использует лишь те функции что нужно динамически подгружая то, что надо используя функционал библиотеки отдельно от нашей основной программы.

```
                           ___________________________________________
			   |                                         |
                           v                                         |
DLL       ПРОГА           DLL                  ПРОГА                 |
_______   ______  Д   ____________      __________________________   |
|ф-ции | |      | И  | (фция_1)<--|--   | 1.  LoadLibrary()-------|--| 
|библы | |      | => |            | |---|-2.1 GetProcAddr(фция_1) |
|______| |______| Н  | (фция_1)<--|-----|-2.2 GetProcAddr(фция_2) |
                  А  |            |  |--|-2.N GetProcAddr(фция_N) |
		  М  | (фция_N)<--|--|  | 3.  FreeLib()           |
		     |____________|     |_________________________|
```




	
### Ошибки в программе под ОС Windows

Информация об ошибках
0. Логирование в файл событий  
+ рабочее решение
- замедляет работу программы
- нужно вовремя включить
- не всегда есть гарантия записи в файл

Две категории ошибок:
- 1) Падение
- 2) Зависание

- Дедлок / Поток / Процесс завис / зацикливание  
- Если зависло у нас - то на этапе дебага, можем использовать WinDbg -> Attach to process
- Если процесс завис у пользователя можем попробовать сделать то же самое, НО есть его у него нет
- Через диспетчер задач он может сказать какой у него процесс завис (имя),
- Process Explorer может выдать нам выдать про процесс более подробную инфу.
- Пользователь не имеет прав на УСТАНОВКУ, так что без установки стороннего ПО и нельзя через TeamViewer получить доступ на ПК, у него ЕСТЬ права на Запуск.

Девочка / Мальчик из тех поддержки:

------------------------------------------------------------------
111]  ЗАВИСАНИЯ

1) Без подключения и без запуска:

- EventLog виндовый посмотреть
- Создать файл дампа через Диспетчер задач - на рабочем зависшем или независшем короче ещё работающем процессе.
Снять несколько "отпечатков/слепков"файлов дампа процесса. По ним (10 штук) можно примерно понять что происходит.

2) С запуском чего-то от нас:
- ProcessExplorer - Дампы + общая миниинфа
- ProcessMonitor - Полный лог работы этого процесса
    -- куда лезет
    -- какие мьютексы открывает
    -- call стеки какие вызывает
	
Полное логирование в общем + дамп => отсюда можно практически всё понять в чём проблема.

ProcessMonitor - профилировщик удалённый, который аттачится к процессу.

МИНУС: Нужны права на установку этого софта.

------------------------------------------------------------------
222]  ПАДЕНИЯ

222.1] В режиме ОС:
- PageFault
- AccesVialation

http://citforum.ru/programming/delphi/delphi_bugs.shtml

AccesVialation
- имя библиотеки / имя dll - где оно произошло ПО такому-то Адресу:
0х123... Как найти процедуру функции, в которой произошло это падение - 
```
Access violation at address <HEX_value>
in module <Application.Exe>.
Read of address <HEX_value_2>
```
- 1. Статический анализ ситуации - чтобы понять ГДЕ - ТОЧКА
- 2. А по ТОЧКЕ делать какие-то выводы.

Если второй адрес при выдачи ошибки - FFFFFFF (или 0000000) Вы можете почти утверждать, что было обращение к объекту, который еще не был создан. Например, вызов метода формы, которая не была создана.
```
procedure TfrMain.OnCreate(Sender: TObject);
var BadForm: TBadForm;
begin
	BadForm.Refresh; // причина  ошибки
end;
```
	
DLL: Во время сборки проекта так же 
- lib
- idl
- map

- PDB - Programm Data Base - инфа о скомпилиоованной программе.
    -- вызовы функций
    -- параметры
    -- смещеня 

В случае анализа ДАМПА + PDB файл становится очень информативен.
- map - Файл текстовый со всемми точками входа для всех функций, сгенерированных в рамках данной библиотеки
- У каждой точки входа есть своё смещение 

ИТАК Получив Адрес из Исключения - Мы открывваем МЭП файл, там написано с какого смещения начинается какая функция
мы берём этот адрес смотим в какой диапазон / функцию он попадает  => получаем имя функуции и смещение относительно этой функции
открываем дизасемблер смотрим по кодам и получаем кокнретную точку, где и что происзошло, с какими ПАРАМЕТРАМИ.

Это СТАТИЧЕСКИЙ анализ - можем попробовать повторить ситуацию, повызывать функцию именно с такими параметрами и попытка воспросизвести ошибку.


222.2] Внутренние ошибки:
- Exeptions
- Некорректное поведение (внутренняя логика ПО)

Кокнретный шаг - Exception 
- имя библиотеки / имя dll - где оно произошло 
- Адрес, который нам это исключение выдало 



## УТЕЧКИ ПАМЯТИ - MEMORY LEAKS DETECTION - РАЗРАСТАНИЕ ПАМЯТИ ПРОЦЕССА ВО ВРЕМЯ РАБОТЫ

https://habrahabr.ru/post/82514/

CRT

https://rsdn.org/article/vcpp/leaks.xml

Активизация режима обнаружения утечек памяти

Самые главные ваши помощники в борьбе с утечками памяти - отладчик и функции отладочной "кучи" из CRT. Чтобы активизировать эти функции, нужно включить в программу следующие строки:

```
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
```
После того, как вы добавили в вашу программу указанные строчки, вы можете получить информацию об утечках памяти, обратившись к функции
```
_CrtDumpMemoryLeaks();
```
Когда программа выполняется под управлением отладчика, _CrtDumpMemoryLeaks отображает информацию об утечках памяти на вкладке Debug окна Output. Эта информация выглядит примерно так:
```
Detected memory leaks!
Dumping objects ->
C:\PROGRAM FILES\VISUAL STUDIO\MyProjects\leaktest\leaktest.cpp(20) : {18} normal block at 0x00780E80, 64 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 
Object dump complete.
```
Если бы вы не включили в программу директиву #define _CRTDBG_MAP_ALLOC, отчёт выглядел бы так:
```
Detected memory leaks!
Dumping objects ->
{18} normal block at 0x00780E80, 64 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 
Object dump complete.
```
Как видите, _CrtDumpMemoryLeaks выдаёт гораздо более полезную информацию, когда символ _CRTDBG_MAP_ALLOC определён. Без него вам выдаются следующие данные:

- порядковый номер распределения памяти (в фигурных скобках).
- тип блока (нормальный, клиентский или CRT-блок).
- адрес блока (в шестнадцатеричном представлении).
- размер блока в байтах.
- содержимое первых 16 байт этого блока (также в шестнадцатеричном представлении).

Если _CRTDBG_MAP_ALLOC определён, вам дополнительно показывается имя файла, в котором произошло распределение памяти. После имени файла в скобках содержится номер строки (20 в нашем примере). Если сделать двойной щелчок на строчке, содержащей имя файла и номер строки в нём:
```
C:\PROGRAM FILES\VISUAL STUDIO\MyProjects\leaktest\leaktest.cpp(20) : {18} normal block at 0x00780E80, 64 bytes long.
```	
то курсор переместится на строку в файле с исходным кодом программы (в нашем примере строку 20 в файле leaktest.cpp), где произошло распределение памяти. Аналогичного эффекта можно добиться, выделив строчку и нажав F4.

Использование _CrtSetDbgFlag

Если ваша программа всегда завершается в одном и том же месте, использовать _CrtDumpMemoryLeaks достаточно просто. А что делать, если программа может завершаться в различных местах? Вместо того, чтобы расставлять по всей программе вызовы _CrtDumpMemoryLeaks, вы можете разместить в начале программы следующий код:

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
После этого функция _CrtDumpMemoryLeaks будет автоматически вызываться перед завершением работы вашей программы. Вы должны задать оба флага (_CRTDBG_ALLOC_MEM_DF и _CRTDBG_LEAK_CHECK_DF), как это показано выше.

- Установка режима сообщений CRT
- Установка точки останова на нужном распределении памяти
- Сравнение состояний памяти

Чтобы определить, не было ли утечек памяти на каком-то участке вашей программы, вы можете сделать "снимки" памяти до и после этого участка, а затем вызвать _CrtMemDifference, чтобы сравнить два состояния:
```
_CrtMemCheckpoint( &s1 );
// memory allocations take place here
_CrtMemCheckpoint( &s2 );

if ( _CrtMemDifference( &s3, &s1, &s2) ) 
   _CrtMemDumpStatistics( &s3 );
```

===================================================================
	
# LINUX OS / OS LINUX / LINUX QUESTIONS
	
### Команды Linux
https://www.hostinger.ru/rukovodstva/osnovnyje-komandy-linux#2-cd

- cd - переход в папку полный путь до которой мы укажем или в папку относительно той в которой мы сейчас или в папку на уровень выше (cd ..)
- ls - выведет в консоль содержимое текущего каталога (все папки и файлы, что есть в папке в которой находимся)
- cat - вывод в командную строку содержимое файла, который мы укажем после команды cat. Например: "cat file.txt"
- grep - выполнить поиск по всему тексту (содержимому) в одном или нескольких файлах. Например, grep ubuntu notepad.txt будет искать слово ubuntu в файле notepad.
- sudo - команда для выполнения команд, с правами администратора или root. (Сокращение от «SuperUser Do»)
- head - выводит первые N (по умолчанию 10) строк текстового файла.
- tail - выводит последние N (по умолчанию 10) строк текстового файла.
- chmod - изменения разрешений на чтение, запись и выполнение файлов и каталогов (Пример: chmod 777 file.txt)
- kill - команда принудительного завершения программы (=процесса)
- echo - команда выводит в консоль текст, который будет стоять справа от неё (echo Здравствуйте, меня зовут Джон). Вывод текста можно так же направить в файл echo Здравствуйте, меня зовут Джон >> name.txt
- man - выведет инструкцию по пользованию любой командой, указанной справа от man. (Например, man tail покажет инструкцию к команде tail)
- pwd - выведет полный путь до папки в которой мы сейчас находимся
- touch - создать пустой файл с именем, которое укажем после команды touch
- mkdir - создать пустую папку с именем, которое укажем после команды mkdir
- ssh - ssh user_name@192.168.10.2 - выполнит вход под учёткой с именем user_name в машину работающую по IP-адресу 192.168.10.2 по сети
- scp - scp /home/ivan/file_1.txt user_name@192.168.10.2:/home/alex/file_1.txt копирование файла по сети, с одного Пк, на другой, к которому мы укажем имя пользователя@айпишник:и_полный_путь_куда_скопировать_файл
- apt-get install - команда для установки программ через командную строку (пользоваться надо так (например чтобы установить КУРЛ): sudo apt-get install curl)	

https://www.interviewbit.com/linux-interview-questions/

### What do you mean by Linux? Explain its features. 

Linux is a Unix-like open-source computer operating system (OS) that directly manages hardware and resources of a system such as CPU, memory, and storage, and manages the communication between software and hardware. It was first released on 5 October 1991 by Linus Torvalds for computers and is considered more secure and faster than Windows. It is freely distributable and is generally built around Linux Kernel (low-level system software that is used to manage hardware resources for users). Furthermore, it can be installed in mobiles, laptops, computers, notebooks, etc. Flavors of Linux OS include Ubuntu, Debian, SUSE Linux, Gentoo, etc.  

Some important features of Linux OS include: 

    Free and Open Source: It is freely and easily available to anyone.
    Robust and Adaptable: Linux can operate for prolonged periods without crashing and considered to have very much immune to security threats.
    More secure: It is more secure as it provides security using authentication features like password authentication, security auditing, and file system access control.
    Multiprogramming System: More than one can function or an application can run simultaneously.
    Application Support: It has its own software repository i.e., place or storage location from where software can be retrieved so that users can download and install applications.
    Supports customized keywords: There are many different languages used throughout the world, therefore Linux supports the installation of multiple languages keyboards.
    GUI (Graphical User Interface): It provides an interface for users to interact with the system and allows to use of GUI applications such as VLC, Firefox, etc.

Enhance your chances of performing well in the interviews with our comprehensive guide, which covers important topics on Linux to help freshers, as well as experienced candidates, ace their interviews.
Crack your next tech interview with confidence!
Take a free mock interview, get instant⚡️ feedback and recommendation💡
Attempt Now
Events | Powered By

    Software Dev
    Data Science
    All Events

No More Events to show!
View All
Linux OS internals

### 1. What are basic elements or components of Linux?

Linux generally consists of five basic elements or components as given below: 

    Kernel: It is considered a core or main part of Linux and is generally responsible for all major activities of OS such as process management, device management, etc.  
    System Library: These are special functions or programs with the help of which application programs or system utilities can access features of the kernel without any requirement of code. It is simply used to implement the functionality of the OS. 
    System Utility: These are utility programs that are responsible to perform specialized and individual-level tasks. They are considered more liable and allow users to manage the computer.  
    Hardware: It is physical hardware that includes items such as a mouse, keyboard, display, CPU, etc. 
    Shell: It is an environment in which we can run our commands, shell scripts, and programs. It is an interface between user and kernel that hides all complexities of functions of the kernel from the user. It is used to execute commands. 

### 2. What is BASH?

BASH (Bourne Again Shell) is basically a command language interpreter. It was written by Brian Fox for GNU OS and can be used in place of Bourne Shell. It is similar to Bourne Shell but includes some additional features such as command-line editing that make it easier and more convenient to use. It is the default user shell on most Linux installations. It is basically an interpreted and non-compiled process that can also run in the terminal window. It is also capable of reading commands from shell scripts.

### 3. What is Kernel? Explain its functions.

A kernel is considered the main component of Linux OS. It is simply a resource manager that acts as a bridge between hardware and software. Its main role is to manage hardware resources for users and is generally used to provide an interface for user-level interaction. A kernel is the first program that is loaded whenever a computer system starts. It is also referred to as low-level system software.

Its other main functions include: 

    Memory Management
    Process Management
    Device Management
    Storage Management
    Manage access, and use of various peripherals that are connected to the computer.

You can download a PDF version of Linux Interview Questions.
Download PDF

### 4. What are two types of Linux User Mode?

There are two types of Linux user mode as given below:  

    Command Line 
    GUI  

### 5. What is LILO?
LILO (Linux Loader) is basically a bootloader for Linux that is used to load Linux into memory and start the OS. It is also known as a boot manager that facilitates a dual boot of a computer. It can function as either a master boot program or secondary boot program and performs various functions such as locating kernel, identifying other supporting programs, loading memory, and starting the kernel. If you want to use Linux OS, then you need to install a special bootloader i.e., LILO for it as it allows fast boot of Linux OS.

### 6. What is swap space?
Swap space, as the name suggests, is basically a space on a hard disk that is used when the amount of physical memory or RAM is full. It is considered a substitute for physical memory. Its main function is to substitute disk space for RAM memory when real RAM does not have enough space to hold all programs that are executing, and more space is required. In simple words, it can be used as an extension of RAM by Linux. 
### 7. What do you mean by a Process States in Linux?
Linux Process is a type of process that can be in a number of different states. The process enters these states from start till end. Process states in Linux are as follows:  

    New/Ready: In this state, a new process is created and is ready to run.
    Running: In this state, the process is being executed.
    Blocked/Wait: In this state, the process is waiting for input from the user and if doesn't have resources to run such as memory, file locks, input, then it can remain in a waiting or blocked state.
    Terminated/Completed: In this state, the process has completed the execution or terminated by the OS.
    Zombie: In this state, the process is terminated but information regarding the process still exists and is available in the process table.

### 8. What is Linux Shell? What types of Shells are there in Linux?
Linux shell is a user interface present between user and kernel. It is used for executing commands and communication with Linux OS. Linux shell is basically a program used by users for executing commands. It accepts human-readable commands as input and converts them into kernel understandable language. 

Different types of shells are commonly used on typical Linux system as listed below: 

    CSH (C Shell)
    KSH (Korn Shell)
    BASH (Bourne Again Shell)
    TCSH
    ZSH
    Bourne Shell

### 9. Name different types of modes used in VI editor.
VI editor (Visual Editor) is basically a default text editor that usually comes with most of the Linux OS. There are basically three types of modes used in VI editor as given below: 

    Command Mode/Regular Mode: It is the default mode for the vi editors. It is generally used to type commands that usually perform particular or specific vi functions. To enter this mode from another mode (Insert mode), one must press [esc]. In simple words, it lets you view the content.  
    Insertion Mode/Edit Mode: This mode allows you to do text editing, or type text into a file. To enter this mode from another mode (command mode), one must press [esc]. In simple words, it lets you delete or insert text or content. 
    Ex Mode/Replacement Mode: This mode is generally used to save the files and execution of the commands. It basically executes files with different parameters. To enter this mode, one must press [:]. In simple words, it lets you overwrite content or text. 

### 10. What is a maximum length for a filename under Linux?
The maximum length for a filename under Linux is 255 bytes. 

### 11. Name the Linux that is specially designed by Sun micro system.
Linux that is specially designed by Sun micro system is Solaris.

### 12. Under the Linux system, what is the typical size for swap partitions?
The typical size for a swap partition under a Linux system should be twice the amount of physical memory or RAM available on the system.

### 13. What are file permissions in Linux? Name different types of file systems in Linux.

There are three owners in the Linux System i.e., user, group, and others. These owners have three types of permissions defined as listed below: 

    Read (r): It allows the user to open and read the file or list the directory. 
    Write (w): It allows the user to open and modify the file. One can also add new files to the directory. 
    Execute (x): It allows the user to execute or run the file.  One can also lookup a specific file within a directory. 

### 14. Name the file that is used to automatically mount file systems.

File that is used to automatically mount file systems is a Fstab file. 
15. What is LVM and why is it required?

LVM (Logical Volume Management) is basically a tool that provides logical volume management for the Linux kernel. It is being introduced simply to make physical storage device management easier. It also includes allocating disks, striping, mirroring, resizing logical volumes. Its main advantages are increased abstraction, flexibility, and control. It simply allows for flexible disk space management. It is especially required to resize the size of the file system online. In Linux, the size of the LVM partition can be extended using “lvextend” command and can be reduced using “lvreduce” commands, respectively.  
16. What is a “/proc” file system?

Proc file system is a pseudo or virtual file system that provides an interface to the kernel data structure. It generally includes useful information about processes that are running currently. It can also be used to change some kernel parameters at runtime or during execution. It is also regarded as a control and information center for the kernel. All files under this directory are named virtual files. 

### 17. What do you mean by the daemons?
Daemons also referred to as the background process, is a long-running Linux program that runs in the background. They do not have any controlling terminal, therefore, they run in the background. These are the processes that are generally started when the system is bootstrapped and terminate or end only when the system is shut down. It is simply the way of extending the functionality of the base OS. It provides and offers several functions that are not available in OS. Its main purpose is to handle periodic requests and then forward the requests to the appropriate programs for execution.  
18. Name daemon that controls the print spooling process.

The daemon that controls that print spooling process is the Line printing daemon.

### 19. What is a Zombie Process?
Zombie Process, also referred to as a defunct or dead process in Linux, is a process that has finished the execution, but its entry remains in the process table. It usually happens due to a lack of correspondence between parent and child processes. This process occurs for the child process because the parent process needs to read the status of the child process. Once it is completed using the wait system call, this process is removed from the process table.

### 20. What is the difference between cron and anacron?
- Cron: It is a program in Linux that is used to execute tasks at a scheduled time. It works effectively on machines that run continuously. 
- Anacron: It is a program in Linux that is used to execute tasks at certain intervals. It works effectively on machines that are powered off in a day or week. 
Cron 	Anacron 
It is a daemon. 	It is not a daemon. 
It can be scheduled by any normal user.  	It can only be used by super users.
It is considered ideal for servers.  	It is considered ideal for desktops and laptops.
It expects the system to run 24*7 	It does not expect the system to run 24*7
Its minimum granularity is in minutes.  	Its minimum granularity is only in days
It is used to execute scheduled commands. 	It is used to execute commands periodically.

### 21. What is load average in Linux?
Load average, as the name suggests, is the average system load on Linux servers being calculated over a given period of time. The load average of Linux servers can be found using “top” and “uptime” commands. It is simply used to keep track of system resources. It is represented by a decimal number starting at 0.00.  It tells you the load that the system has been under.

### 22. What do you mean by Shell Script?
Shell Script, as name suggests, is a script especially written for shell. Here, script means programming language that is being used to control applications. It simply allows the execution of different commands that are entered in the shell.  It generally helps you to create complex programs containing conditional statements, loops, and functions. It is very easy to debug, can simplify everyday automation processes, and is much quicker as compared to writing big programs.

### 23. What is INODE and Process Id?
INODE: It is a unique name given to each file by OS. Each inode has a unique inode number within a file system. It stores various information about files in Linux such as ownership, file size, file type, access mode, number of links, etc.  
- Process Id (Identifier): It is a unique Id given to each process. It is simply used to uniquely identify an active process throughout the system until the process terminates.

### 24. Name the first process that is started by the kernel in Linux and what is its process id?
The first process started by the kernel in Linux is “init” and its process id is 1.

### 25. What is CLI and GUI?
CLI (Command Line Interface): It is basically a command-line program that usually accepts text as input to execute or run functions of the operating system. It allows users to type declarative commands simply to give instructions to the computer to perform or execute operations. It usually requires less memory to use as compared to other interfaces as well as it does not require Windows and a low-resolution monitor can be used. It usually provides greater flexibility of use and can also be used to perform things easily that are most difficult to do with GUI.  

GUI (Graphical User Interface): It is basically a human-computer interface that allows users to interact with electronic devices through graphical icons and visual indicators. The use of these graphical elements or icons makes it easier for users to interact with the system. It is visually intuitive and allows higher productivity. It is usually a combination of graphical and textual interaction that uses menus, buttons, message boxes, etc. 
Linux Networking

### 26. Why /etc/resolv.conf and /etc/hosts files are used?
- /etc/resolv.conf: It is used to configure DNS name servers as it contains the details of the nameserver i.e., details of your DNS server. The DNS server is then used to resolve the hostname of the IP address. 
- /etc/hosts: It is used to map or translate any hostname or domain name to its relevant IP address. 

### 27. What are the advantages of using NIC teaming?
- NIC (Network Interface Card) teaming has several advantages as given below: 

- Load Balancing  
- Failover  
- Increases uptime

### 28. What do you mean by Network bonding?
Network Bonding, also known as NIC Teaming, is a type of bonding that is used to connect multiple network interfaces into a single interface. It usually improves performance and redundancy simply by increasing network throughput and bandwidth.

### 29. What are different network bonding modes used in Linux?
Different network bonding modes used in Linux are listed below: 

    Mode-0 (balance-rr): It is the default mode and is based on round-robin policy. It offers features like fault tolerance and load balancing. 
    Mode-1 (active-backup): It is based on an active-backup policy. In this, only one node responds or works at the time of failure of other nodes. 
    Mode-2 (balance-xor): It sets an XOR (exclusive-or) mode for providing load balancing and fault tolerance.  
    Mode-3 (broadcast): It is based on broadcast policy. It sets a broadcast mode for providing fault tolerance and can be used only for specific purposes.  
    Mode-4 (802.3ad): It is based on IEEE 802.3ad standard also known as Dynamic Link Aggregation mode. It sets an IEEE 802.3ad dynamic link aggregation mode and creates aggregation groups that share the same speed and duplex settings. 
    Mode-5 (balance-tlb): It is also known as Adaptive TLB (Transmit Load Balancing). It sets TLB mode for fault tolerance and load balancing. In this mode, traffic will be loaded based on each slave of the network.  
    Mode-6 (balance-alb): It is also known as Adaptive Load Balancing. It sets ALB mode for fault tolerance and load balancing. It doesn’t need any special switch support.  

### 30. Name default ports used for DNS, SMTP, FTP, SSH, DHCP and squid.

Default ports used for various services are as follows:
Service  	Port 
DNS 	53
SMTP 	25
FTP 	20 (Data transfer), 21 (Connection established)
SSH 	22
DHCP 	67/UDP (dhcp server), 68/UDP (dhcp client) 
squid 	3128

### 31. What is SSH? How we can connect to a remote server via SSH.
SSH (Secure Shell), as the name suggests, is basically a protocol that is being used to securely connect to remote servers or systems and enables two systems to communicate. It is considered the most common way to have access to remote Linux servers. It generally transmits data over encrypted channels therefore security is considered at a high level. To connect to a remote server via SSH, you need to own a domain name and IP address.

### 32. Write the difference between Soft and Hard links?
- Hard Links: It is a special kind of file that points to the same underlying inode as another file. It can be referred to as an additional name for an existing file on Linux OS. Total number of hard links for a file can be displayed using the “ls -l” command. Such links cannot be used across file systems. Hard links can be created using the following command: 
$ ln [original filename] [link name] 

- Soft Links: It is also termed a symbolic Link. Soft links are kinds of files that usually point to another file. It does not include any amount of data in the target file and simply points to another entry anywhere in the file system. Such links can be used across file systems. Soft links can be created using the following command: 
$ ln -s [original filename] [link name] 
Hard Links  	Soft Links
It is considered a mirror copy of the original file. 	It is considered a symbolic link to the original file.
It usually shares the same inode number. 	It usually shares different inode numbers.
It contains the original contents of the files. 	It does not contain the actual contents but contains the location of the original file.
Any changes made to the original file will directly reflect other files.  	Any changes or modifications made to the soft link will directly reflect the original file and its hard links. 
It cannot be used for linking directories. 	It can be used for linking directories.
Such links are faster as compared to soft links. 	Such links are slower as compared to hard links.

### 33. Name three standard streams in Linux.

Standard streams are basically I/O (Input and Output) communication channels between a program and its environment in Linux. Input and output in the Linux environment are distributed across three standard streams. Three standard streams in Linux are as follows:

    Standard Input (stdin)
    Standard Output (stdout)
    Standard Error (stderr)

Linux Commands

### 34. What is netstat command?
netstat (Network statics) command is generally a networking tool being used for troubleshooting and configuration and used to display all network connections on a system. It simply provides a way to check whether various aspects of TCP/IP are working and what connections are present.

### 35. What is the ping command?
Linux ping (Packet Internet Groper) command is a command that is used to check connection status between source and destination. In simple words, this command is used to check whether a network is available and if the host is reachable. It can also be used to troubleshoot different connectivity issues, verify connectivity at an IP -level to a second TCP/IP device, and name resolution. One can use this command to test both the computer name and IP address of the computer. 

### 36. Which command is used to check the default run level?
The command used to check the default run level is “/etc/inittab”. 

### 37. Which command is used to check the size of file or directory?
The command used to check the size of the file or directory is “du”. Here “du” stands for disk usage that is used to check information of disk usage of files and directories on a machine. It is also used to display files and directory sizes in a recursive manner. 
```
Example:  
$ du -sh /var/log/*
1.8M /var/log/anaconda
384K /var/log/audit
4.0K /var/log/boot.log
0 /var/log/chrony
4.0K /var/log/cron
4.0K /var/log/maillog
64K /var/log/messages
```

38. Which command is used to count the number of characters in a file?
The command that is used to count the number of characters in a file in Linux is “wc”. Here “wc” stands for word count. It is used to count the number of lines, words, and characters in a text file. 

### 39. What is the function of grep command?
Grep (Global regular expression print) is a command that is used to the global search for a string of characters in a specified file. The text search pattern is generally known as a regular expression. It simply makes use of pattern-based searching.  

Syntax: grep [options] pattern [files] 
Example: $ grep -c "linux" interview.txt   

The above command will usually print the total count of the word “Linux” in the file “interview.txt”.

### 40. Explain working of env command.
“env” command is basically a shell command that is used to print a list of current environmental variables. Here, “env” stands for the environment. It can also run another process in another environment without any modification of the current environment. It allows you to run programs in a modified environment. It is generally used by shell scripts to launch the correct interpreter. It can also be useful for checking if wrong environment variables prevent the application from starting during troubleshooting.  
```
Example: 
$env
PHYTHON_PIP_VERSION=9.0L1
HOME=/root
DB_NAME=test
PATH=/usr/local/bin:/usr/local/sbin
LAND=C.UTF=8
PYTHON_VERSION=3.4.6
PWD=/
DB_URI=mongodb://database:27017/test
```

### 41. What is the pwd command?
“pwd” command is basically a command that is used to print the complete path of the current working directory starting from the root (/). Here, “pwd” stands for Print Working Directory. It is considered one of the most basic and most used commands in Linux. This command is usually a built-in shell command and is also available in different shells such as bash, ksh, zsh, bourne shell, etc.

### 42. Name the command that is used to check all the listening ports and services of your machine.
	# netstat -ntlp
	
### 43. Which command is used to check the memory status?

The command used mostly to check memory status in Linux is “free”. Other commands that can be used are given below: 

    “cat” command: It can be used to show or display Linux memory information. (cat/proc/meminfo)
    “vmstat” command: It can be used to report statistics of virtual memory. 
    “top” command: It can be used to check the usage of memory. 
    “htop” command: It can be used to find the memory load of each process. 

### 44. What is pipe?
In Linux, a pipe is basically a form of redirection that is used to send the output of one command to another command for further processing. It simply takes the output from one command and uses it as an input for another. It provides asynchronous execution of commands with help of buffered I/O routines.

### 45. What are Linux directory commands?
There are basically five Linux directory commands that are used to work with files and directories as given below: 

    pwd: It stands for “print working directory”. This command is generally used to display the path of the present or current working directory. 
    Syntax: $ pwd 
    cd: It stands for “change directory”. This command is generally used to change the present working directory to the directory that we want to work on. 
    Syntax: $ cd <path to new directory> 
    Is: It stands for “list”. This command is generally used to show the full list of content of files and directories in the present working directory. 
    Syntax: $ ls 
    mkdir: It stands for “make directory”. This command generally allows users to create directories in Linux.  
    Syntax: $ mkdir <name (and path if required) of new directory> 
    rmdir: It stands for “remove directory”. This command is used to remove/delete each directory that is specified on the command line. 
    Syntax: $ rmdir <name (and path if required) of directory> 

### 46. What do you mean by unmask?

Umask, also known as user file-creation mask, is a Linux command that allows you to set up default permissions for new files and folders that you create. In Linux OS, umask command is used to set default file and folder permission. It is also used by other commands in Linux like mkdir, tee, touch, etc. that create files and directories. 

Syntax: umask [-p] [-S] [mask]  

Where,  
- [mask]: It represents the permission masks that you are applying. 
- [-S]: It displays the current mask as a symbolic value. 
- [-p]: It displays the current mask along with umask command thus allowing it to be copied and pasted as a future input.  

### 47. Name the command used to review boot messages.

The command that is used to review boot messages is the “dmesg” command.

Linux vs Other OS

###  48. Write difference between Linux and Unix.

Linux: It is an open-source and free-to-use Operating system that was first developed in 1991 by Linus Torvalds. It is specially designed to offer free and low-cost OS for personal computer users. It has very low hardware requirements and facilitates powerful support for networking. Different versions of Linux are known as variants. 
Example: Red Hat, Android, etc. 

Unix: It is a closed source OS derived from the original AT&T Unix and was first developed in the 1960s. It is not open source and is not free to use. It is generally capable of handling activities from multiple users at the same time. Furthermore, it is specially designed to be more portable, multi-user, and multitasking in a time-sharing configuration. Different versions of Unix are known as distros. 
Example: HP-UX, Solaris, etc. 
	

===================================================================


-------------------------
# DC0A21C3-D27E-477E-984C-301076DAC9DB

### Каков размер типа char / размер объекта типа char ? ?
sizeof(char) всегда вернёт **1** !

НО! В стандарте C++ не определено, чем же именно является тип char — он может быть как знаковым, так и беззнаковым.

### Чтобы число умножить на 2 какую побитовую операцию можно применить 
Ответ: ПОБИТОВЫЙ СДВИГ Влево
```
0000 0011 = 3
0000 0110 = 6
```
### Разница между оператором [] и оператором at()
- vector.at(i); Делает попытку обратиться к элементу и  Бросает исключение std::out_of_range  при выходе за пределы вектора 
- operator[] делает попытку считать данные и падает без бросания исключения

### Двоичный поиск / Бинарный поиск / Алгоритм бинарного поиска 
Задача: Есть искомый Элемент и Массив в котором этот элемент нужно найти.
- Решение рекурсивное.
- Особенность: Бинарный поиск можно осуществлять внутри отсортированного массива
- Сложность: O(log(n))

Суть: Сравниваем искомый элемент с элементом стоящим всередине и сраниваем с
соседним слева или справа от него.
Соотвественно отбрасываем ту половину амссива относительно элемента, который 
был больше искомого и продолжаем поиск в другой половине.
Снова выбираем опорный элемент всередине отобранной середины и продолжаем 
поиск до тех пор пока не найдём искомый постоянно ополовинивая зону поиска.

### Дан Связный список - узнать закольцован он или нет ? 

https://en.wikipedia.org/wiki/Cycle_detection

Известно несколько алгоритмов поиска цикла быстро и при малой памяти:
1. Алгоритм «Черепахи и Зайца» Флойда передвигает два указателя с различной скоростью через последовательность значений, пока не получит одинаковые значения
2. Алгоритм Брента, основан на идее экспоненциального поиска[en].

Решение тут: Sample Cycles test for Node in a List

- Если коротко, то надо "пустить" по списку два указателя, один вдвое быстрее другого. Либо список кончится, либо указатели встретятся.
- В каждом элементе указатель next поменять так что бы он указывал на предыдущий. По достижению null, если текущий элемент равен начальному то цикл есть иначе нет.. Что бы востановить первоначальный список, надо снова пройтись и поменять указатели обратно.
- Моё первое решение: Сохранить начало списка, итеративно перебирать элементы списка, начиная с первого и проверять за сколько шагов с начала доступен текущий елемент [C] и тот, который является следующим за текущим [N]. Если N<C — значит елемент с номером N — начало цикла. Итого: память O(1), сложность O(n^2)
- Второе решение: Сохраняем первый и второй елемент списка, и итеративно меняем указатели каждого елемента списка на послеследующий елемент. Делаем так, пока не встретим конец списка, либо попадём в ситуацию, когда послеследующий является текущим — это признак цикла. Свернуть список обратно, имея 1-й и 2-й елементы просто, т.к. они чередуются. Итого: память O(1), сложность O(n)
- есть классический алгоритм, когда по списку пускаем 2 указателя, один за шаг переходит на 1 элемент вперед, 2-й - на 2 элемента вперед. При проходе 2-м проверяем, если он указывает на тот-же объект, что и первый - значит цикл есть.

### Если для этой задачи мы сильно увеличим входные объёмы, то что может произойти ? 
Сложность операций увеличится на ???

### const expression разница в 11 и в 17 стандарте ?
- В 17-той версии внутри const expression можно делать циклы (в 11той этого делать нельзя) - и поэтому в 17-том Задача "SWITCH CASE" со строками она решается рекурсией. (Рекурсия для рантайма это плохо, а для компайл тайма ещё более менее пойдёт.)

### Двоичное дерево поиска

Есть бинарное / Двочиное дерево, если оно сбалансированно - O(log по основанию 2 от n)

### Сбалансированные Двоичное деревья и разница между двоичными сбалансрованными деревьями?

- B-дерево
- AVL-дерево
- RB-дерево / Красно-чёрное дерево

РАЗНИЦА: Разница в том как деревья **балансируются**

### Хэш-таблица Зачем она нужна, в чём её преимущества и недостатки

При формировании этой структуры даннх нам не нужно постояно заботиться в отличие от дерева, о том, чтобы постоянно "держать баланс" при добавлении каждого онового элемента.

Всё же операция добавления элемента в случае когда НЕТ коллизиц равная О(1) против О(log n) для дерева, существенна!

Одновременно с этим если элементов огромное число, то и скорость работы хэш функции встаёт как вопрос, и вопрос хранения всех Хэш-индексов под хэш таблицу .... Тоесть проблемы с памятью мы можем начать испытывать.

В случае коллизий отдельная проблема, из-за формирования Бакетов, со всеми вытекающими проблемами списков, которые формируются на этом.

### Хэш-таблица 2 основных типа организации

https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0

- Хэш-таблица ЗАКРЫТОГО ТИПА (Формирование Бакетов - типа СИПСОК - методом цепочек)
- Хэш-таблица ОТКРЫТОГО ТИПА (Формирование Бакетов - типа пара ключ-значение - методом пробирования)

Различают хэш таблицы по тому как в них решается проблема Коллизий

1. Метод цепочек 
- Формирование Бакетов - Каждая ячейка массива H является указателем на связный СПИСОК (цепочку) пар ключ-значение, соответствующих одному и тому же хеш-значению ключа. Коллизии просто приводят к тому, что появляются цепочки длиной более одного элемента.
2. Открытая адресация 
- В массиве H хранятся сами пары ключ-значение. Алгоритм вставки элемента проверяет ячейки массива H в некотором порядке до тех пор, пока не будет найдена первая свободная ячейка, в которую и будет записан новый элемент. Этот порядок вычисляется на лету, что позволяет сэкономить на памяти для указателей, требующихся в хеш-таблицах с цепочками.
Последовательность, в которой просматриваются ячейки хеш-таблицы, называется последовательностью проб

### Какие типы Индексов вам известны

1. Хэш таблицы 
2. Б деревья
3. Reversed index
4. Clustered index

### FTS - Full Text Search - как организован ? 
Задача: по входному слову нужно найти либо предложение с этим словом либо указать строку с этим словом в тексте.

Решение: Надо найти объект из каких-то критериев. Критерий может быть - конкретное поле и мы это значение будем индексировать в зависимости от уникальности данных и нужно ли упорядоченное чтение или нужно найти кокнретный объект. Для решения мы соотвественно используем хэш-таблицу или двоичное дерево. 

### Reversed index 
Будет фактически множественным индексом на каждую запись. Разобрали текст по словам, указатель на слово в дерево, и далее мы продолжаем строить наше дерево.

### Clustered index (группировка данных)
Например есть двусвязный список, и мы формируем "кластер". Ибо быстрее будет найти вхождение в кластер по индексу по хэш таблице, далее мы проходя по кластеру, в случае если в нём все данные упорядочены. В том случае если перед нами стоит задача сгруппировать данные по какому-то одному признаку, то организация хранения данных совметно имеет место быть. Всё это делается для того, чтобы Уменьшить количество циклов чтения данных.

https://ru.wikipedia.org/wiki/%D0%9F%D0%BE%D0%BB%D0%BD%D0%BE%D1%82%D0%B5%D0%BA%D1%81%D1%82%D0%BE%D0%B2%D1%8B%D0%B9_%D0%BF%D0%BE%D0%B8%D1%81%D0%BA

https://habr.com/ru/post/40218/

Reversed Index - использовал бы двоичное дерево поиска внутри себя

https://stackoverflow.com/questions/3073390/create-and-use-html-full-text-search-index-c/3130822

Indexing
The map word ==> page data structure is known as an inverted index. Its inverted because its often generated from a forward index of page ==> words. Inverted indexes generally come in two flavors: inverted file index, which map words to each document they occur in, and full inverted index, which map words to each position in each document they occur in.

The important decision is what backend to use for the index, some possibilities are, in order of ease of implementation:

SQLite or Berkly DB - both of these are database engines with C++ APIs that integrated into a project without requiring a separate server process. Persistent databases are essentially files, so multiple index sets can be search by just changing the associated file. Using a DBMS as a backend simplifies index creation, updating and searching.
In memory data structure - if your using a inverted file index that is not prohibitively large (memory consumption and time to load), this could be implemented as a std::map<std::string,word_data_class>, using boost::serialization for persistence.
On disk data structure - I've heard of blazingly fast results using memory mapped files for this sort of thing, YMMV. Having an inverted file index would involve having two index files, one representing words with something like struct {char word[n]; unsigned int offset; unsigned int count; };, and the second representing (word, document) tuples with just unsigned ints (words implicit in the file offset). The offset is the file offset for the first document id for the word in the second file, count is the number of document ids associate with that word (number of ids to read from the second file). Searching would then reduce to a binary search through the first file with a pointer into a memory mapped file. The down side is the need to pad/truncate words to get a constant record size.
The procedure for indexing depends on which backend you use. The classic algorithm for generating a inverted file index (detailed here) begins with reading through each document and extending a list of (page id, word) tuples, ignoring duplicate words in each document. After all documents are processed, sort the list by word, then collapsed into (word, (page id1, page id2, ...)).

The mifluz gnu library implements inverted indexes w/ storage, but without document or query parsing. GPL, so may not be a viable option, but will give you an idea of the complexities involved for an inverted index that supports a large number of documents.

### Что такое Фильтр Блума и зачем он нужен ?

https://neerc.ifmo.ru/wiki/index.php?title=%D0%A4%D0%B8%D0%BB%D1%8C%D1%82%D1%80_%D0%91%D0%BB%D1%83%D0%BC%D0%B0

https://habr.com/ru/post/112069/

Это вероятностная структура данных. 

Задача - есть элемент. Вопрос в том лежит этот элемент в множестве или нет ?

Гарантия при ответе такая - Гарантия того, что возможно элемент в множестве находится.

Пример использования: Например, Гуголь использует фильтры Блума в своей BigTable для уменьшения обращений к диску. Оно и не удивительно, ведь по сути, BigTable — это большая очень разреженная многомерная таблица, поэтому большая часть ключей указывает в пустоту. К тому же, данные распиливаются на относительно небольшие блоки-файлы, каждый из которых опрашивается при запросах, хотя может не содержать требуемых данных.

### Виды ввода вывода / Основные методы ввода вывода (I/O)
https://tproger.ru/translations/diversity-of-input-output-io/

- блокирующий
- неблокирующий
- мультиплексированный
- асинхронный

Виды ввода-вывода и примеры функций ввода-вывода Unix:
| -           | Блокирующий |     Неблокирующий           |
| ------------|:-----------:|:---------------------------:|
| Синхронный  | write, read | write, read + poll / select |
| Асинхронный | -           | aio_write, aio_read         |

(Он тоже блокирует поток на операциях ввода-вывода, но вместо того, чтобы производить блокировку по очереди, вы можете запланировать все операции ввода-вывода, которые вам нужно сделать, и блокировать их все. Операционная система разбудит поток, когда какая-нибудь из операций завершится.)

(форма неблокирующей обработки ввода/вывода)

### Каждый thread под отдельный read/recieve ? Чепм это плохо, в линуксе в windows ?
Ответ: Переключение контекста из user mode в kernel mode удут сильно затратными

### Проблема 1: Какие механизмы асинхронного ввода вывода ?

https://ru.wikipedia.org/wiki/%D0%90%D1%81%D0%B8%D0%BD%D1%85%D1%80%D0%BE%D0%BD%D0%BD%D1%8B%D0%B9_%D0%B2%D0%B2%D0%BE%D0%B4-%D0%B2%D1%8B%D0%B2%D0%BE%D0%B4

- Функции обратного вызова
- Корутины
- Порты (очереди) завершения
- Каналы ввода/вывода

ПРИМЕРЫ:
- Функции обратного вызова - Доступны в FreeBSD, OS X, VMS и Windows. - Потенциальная проблема в том, что глубина стека может расти неконтролируемо, поэтому чрезвычайно ключевая вещь, необходимо, только тогда запланировать другой ввод/вывод когда завершён предыдущий. Если это должно быть удовлетворено немедленно, первоначальный callback не выполняет «раскрутку» стека до того, как следующий вызывается.
- Корутины (сопрограммы) позволяют писать асинхронные программы в синхронном стиле. Примеры: async / await. Доступен в C#, Python, EcmaScript. Генераторы. Доступны в PHP, Python, Ruby с помощью ключевого слова yield. Также есть множество библиотек для создания сопрограмм (libcoro[3], Boost Coroutine)
- Порты (очереди) завершения - Доступен в Microsoft Windows, Solaris и DNIX. Запросы ввода/вывода выдаются асинхронно, но уведомления о выполнении предоставляются через механизм синхронизационной очереди в порядке их завершения.
- Каналы ввода/вывода - Доступные в мейнфреймах IBM, Groupe Bull и Unisys каналы ввода-вывода предназначены для максимального

### Проблема 1: Чем плохо использовать многопоточность в приложении, когда к этому нет явных предпосылок ? 
- (+) Раскладывание 1 процесс на несколько ядер.
- (-) Операций в многопоточном приложении у нас чаще всего следующие: 

1. **ПЕРЕКЛЮЧЕНИЕ КОНТЕКСТА** 
- в Windows: ПЕРЕКЛЮЧЕНИЕ КОНТЕКСТА из USER mode в KERNEL mode и обратно создание/уничтожение в ядре Windows.
- в Линукс: **SETCONTEXT()** аналогичные по сути операции в Линуксе соотвественно.

2. **L1 L2 L3** - Кэш память.
- Т.е. ранее мы УПИРАЛИСЬ в (Интерфейс Диск-Память) проблему **скорость работы памяти Жёского диска** куда чаще. 

А сейчас мы стали упираться (Интерфейс КЭШ-Память) в проблему **как бы ещё ускорить операции с уже и так быстрой КЭШ ПАМЯТЬЮ**. 
Причина в том, что память на диске медленная, и поэтому нам выгоднее держать код и данные в Кэше. Допуская минимум ветвлений. Именно поэтому правило хорошего тона в С/С++ как можно меньше использовать оператор условного перехода if() {} if () и т.д. А использовать именно результаты булевых операций.

### Фреймы стека. Кадр стека. Организация фреймов на стеке ?

https://www.gunsmoker.ru/2015/02/stack-frames.html

Семейство процессоров x86 (как 32-битных, так и 64-битных) 
использует аппаратный стек процессора для хранения последовательности 
вызова подпрограмм (т.е. процедур, функций и методов) по мере их выполнения. 
Другие процессоры могут использовать для этого и другие способы. 
Стековые фреймы (также иногда называемые "фреймами вызовов", 
"записями активации" или "фреймами активации") представляют собой структуры 
в стеке процессора, которые содержат информацию о состоянии подпрограммы. 
Каждый стековый фрейм соответствует вызову какой-то подпрограммы, которая 
была вызвана, но ещё не завершилась. Например, если подпрограмма DrawLine 
была вызвана подпрограммой DrawSquare и пока ещё выполняется, то верхняя 
часть стека может выглядеть примерно так:

### ФИЧА в С++, которой НЕТУ в Си? Что есть в С++ и чего нету в обычном Си?
Ответ: Обработка исключений try catch. Нам нужно пройтись по "Кадрам" стека, найти call (места возврата) для того, чтобы раск5рутка стека началась. В качестве альтернативы иногда используют libandwint
Если заниматься РеалТайм, то лучше их не использовать.

### Синхронный ввод-вывод
Указываем, что дескриптор неблокирующий и есть:
- 2 позиксовых - механизма (??? не лучшие) 
- есть Линуксовый механизм (сильно хороший) 
- есть BSD-шный механизм.
- windows - WSA-Socket

В классическом WSA-Socket-е механизм таков:
- **БЛОКИРУЮЩИЙ ВВОД/ВЫВОД** - Мы ждём accept на **listen()** (read / write). Соотвественно блокируем, в отдельном потоке запускаем и ждём. Сокет открыт, он listen (слушает) и именно это и есть БЛОКИРУЮЩИЙ ВВОД/ВЫВОД. (В Windows мы для этого используем HANDLE окна и wndProck())
- **НЕБЛОКИРУЮЩИЙ ВВОД/ВЫВОД** - В Линуксе же есть **select()**, **poll()**, **epoll()**
- в BSD есть **kqueue** "кейкью". А так же в линуксе используют **Дескрипторы** при работе ввода-вывода.
- **timerfd** http://ru.manpages.org/timerfd_create/2 / http://man7.org/linux/man-pages/man2/timerfd_create.2.html  
- **eventfd** http://ru.manpages.org/eventfd/2 /  http://man7.org/linux/man-pages/man2/eventfd.2.html

### Epoll (extended poll) 
API мультиплексированного ввода-вывода, предоставляемого Linux для приложений. API позволяет приложениям осуществлять мониторинг нескольких открытых файловых дескрипторов (которые могут быть файлами, устройствами или сокетами, в том числе сетевыми), для того, чтобы узнать, готово ли устройство для продолжения ввода (вывода). Epoll планировался как более эффективная замена вызовам select() и poll(), определёнными в POSIX. Epoll может предоставить более эффективный механизм для приложений, обрабатывающих большое количество одновременно открытых соединений — со сложностью O(1) в отличие от стандартного механизма, обладающего сложностью O(n). Epoll аналогичен системе Kqueue из FreeBSD и также представляет собой объект ядра, предоставляемый для работы в пространстве пользователя в виде файлового дескриптора.

- Проблема select/poll
Для использования select или poll приложение должно передать в ядро полный список всех файловых дескрипторов, в которых оно ожидает появление данных; а ядро, в свою очередь, должно для каждого из переданных элементов проверить состояние дескрипторов и сформировать структуру, описывающую состояние каждого переданного дескриптора. Такой подход не создаст много проблем в условиях десятков или сотен дескрипторов. Тем не менее, производительность в больших сетях заметно падает.

- Описание epoll API
Linux предоставляет следующие вызовы в рамках API:

- epoll_create() — создаёт структуру данных (epoll instance), с которой в дальнейшем идёт работа. Структура одна для всех файловых дескрипторов, за которыми идёт наблюдение. Функция возвращает файловый дескриптор, который в дальнейшем передаётся во все остальные вызовы epoll API.
- epoll_ctl() — используется для управления epoll instance, в частности, позволяет выполнять операции EPOLL_CTL_ADD (добавление файлового дескриптора к наблюдению), EPOLL_CTL_DEL (удаление файлового дескриптора из наблюдения), EPOLL_CTL_MOD (изменение параметров наблюдения), EPOLL_CTL_DISABLE (добавлена в Linux 3.7) — для безопасного отключения наблюдения за файловым дескриптором в многопоточных приложениях
- epoll_wait() — возвращает количество (один или более) файловых дескрипторов из списка наблюдения, у которых поменялось состояние (которые готовы к вводу-выводу).

События, за которыми можно наблюдать с помощью epoll:
- EPOLLIN — новые данные (для чтения) в файловом дескрипторе
- EPOLLOUT — файловый дескриптор готов продолжить принимать данные (для записи)
- EPOLLERR — в файловом дескрипторе произошла ошибка
- EPOLLHUP — закрытие файлового дескриптора

### **kqueue** 
масштабируемый интерфейс уведомления о событиях. Впервые появился во FreeBSD 4.1[1], также поддерживается в NetBSD, OpenBSD, DragonflyBSD, и Mac OS X. Kqueue позволяет пользователям оперативно получать уведомления о событиях в системе.
kqueue предоставляет механизм уведомления процесса о некоторых событиях, произошедших в системе. Впервые этот интерфейс появился в FreeBSD 4.1 и на данный момент присутствует практически во всех BSD системах. (в Linux есть его аналог - epoll()).

Как это работает: процесс с помощью вызова kqueue() получает дескриптор очереди сообщений ядра (в случае ошибки процесс завершается):
```
if ( ( kq = kqueue() ) == -1 )
  exit(1);
```
затем с помощью kevent() устанавливает, сообщения о каких именно событиях он желает получать. Например, мы хотим знать когда файл
(которому соотвествкует дескриптор fd) изменится.

O K-QUEUE - https://www.opennet.ru/base/dev/kqueue_overview.txt.html

O select / poll / epoll - https://habr.com/ru/company/infopulse/blog/415259/

### SSE и c++ / SSE в c++ / SIMD

https://habr.com/ru/post/440566/

Есть класс задач, которые нельзя ускорить за счёт оптимизации алгоритмов, а ускорить надо. 
В этой практически тупиковой ситуации к нам на помощь приходят разработчики процессоров, 
которые сделали команды, позволяющие выполнять операции на большим количеством данных за одну операцию. 
В случае x86 процессоров это инструкции сделанные в расширениях MMX, SSE, SSE2, SSE3, SSE4, SSE4.1, SSE4.2, AVX, AVX2, AVX512.

- Что такое SIMD
SIMD (Single Instruction, Multiple Data) — одиночный поток команд, множественный поток данных. 
В x86 совместимых процессорах эти команды были реализованы в нескольких поколениях SSE и AVX расширениях процессора. 
Команд довольно много, полный список от Intel можно посмотреть на странице software.intel.com/sites/landingpage/IntrinsicsGuide. 
В десктопных процессорах AVX расширения недоступны, поэтому сосредоточимся на SSE.

Для работы с SIMD в C/C++ в код надо добавить
```
#include <x86intrin.h>
```
Итог: На сайте **INTEL**, выбрав нужное расширение процессора SSE. Мы можем получить на сайте доступ к списку особых функций по категориям:
- категория «Compare» (семейство функций __m128i _mm_cmpeq_epi* (__m128i a, __m128i b) умеют проводить сравнение сразу с несколькими элементами за один цикл)
- категории «Set» и «Load».
- Быстрый подсчёта количества бит в числе есть функции _popcnt32 и _popcnt64
- _mm_movemask_epi8 - работа с масками для массива и Т.Д.

В итоге использование именно этих функций через Inetl-овский процессор может дать прирост производительности в х2 и до х5 раз.

### Чем отличается fiber от thread-а отличается (в линуксе это называется SET CONTEXT)?

### С++11 Как можно сделать switch по строке / Использовать строку в switch / строки внутри "case" ?
Ответ: Используя CONST EXPRESSION, начиная с C++11

https://habr.com/ru/post/166201/

ОГРАНИЧЕНИЕ В CASE (строки в 9 символов максимум)
```
SWITCH (month)
{
   CASE("february"):
       days = "28 or 29";
       break;
   DEFAULT:
       days = "?";
       break;
}	   
```
Но в C++11, к счастью, появились более понятные constexpr-функции, значения которых также могут вычисляться компилятором.

1. Нам нужно написать constexpr-функцию, которая бы оперировала числовыми кодами char-символов.

2. Она проверяет, содержит ли известная на стадии компиляции строка только символы из диапазона 0-127, и возвращает false в случае нахождения «запретного» символа. 
```
constexpr bool str_is_correct(const char* const str)
{
	return (static_cast<signed char>(*str) > 0) ? str_is_correct(str + 1) : (*str ? false : true);
}
```

3. функцию из пункта 2 вставим в макрос
```
#define CASE(str)  static_assert(str_is_correct(str), "CASE string contains wrong characters");\
   case str_hash(...)
```
4. Берём хэш-функцию, не имеющую коллизий вообще.

Так как уже установлено, что все символы строки расположены в диапазоне 0-127, то функция будет иметь вид:

Сумма от i=1 до len ( от str[i] * (128^(len-i)) )

Её реализация такова:
```
   typedef unsigned char uchar;
   typedef unsigned long long ullong;

   constexpr ullong str_hash(const char* const str, const uchar current_len)
   {
      return *str ? (raise_128_to(current_len - 1) * static_cast<uchar>(*str)
      + str_hash(str + 1, current_len - 1)) : 0;
   }
```   

При какой же максимальной длине строки эта функция не будет иметь коллизий? Очевидно, лишь тогда, когда полученное ею значение всегда уместится в диапазоне типа unsigned long long (также окончательно введённого в C++11), то есть если оно не превышает 264-1.

5. Вот для подсчёта хэша в макросе SWITCH нам придётся сделать отдельную функцию
```
#define SWITCH(str)  switch(str_hash_for_switch(str))
const ullong N_HASH = static_cast<ullong>(-1);  // по аналогии с std::string::npos
	
inline ullong str_hash_for_switch(const char* const str)
{
	return (str_is_correct(str) && (str_len(str) <= MAX_LEN)) ? str_hash(str, str_len(str)) : N_HASH;
}
```


	
===================================================================

	
# TASKS / ЗАДАЧКИ

1. Развернуть число
2. Развернуть строку
3. Возведение в степень
4. Фактореал
5. перевод строки в число = atio "своими руками"
6. перевод числа в строку
7. Из строки с пробелами удалить все пробелы в строке
8. Найти подстроку в строке
9. Алгоритм Бинарного поиска / Бинарный/Двоичный поиск С++
10. Поиск пузырьком
11. Развернуть linked_list
12. split / split_string / split string своими руками
- unique_ptr - своими руками
- shared_ptr - совими руками

### 1. Развернуть число

```
int reverseStr(int& n) //tCmplx: O(N) //Space: O(1)
{
  int rev = 0, remainder = 0;
  while(n != 0) {
    remainder = n % 10;
    rev = rev * 10 + remainder;
    n /= 10;
  }
  return rev;
}
```

### 2. Развернуть строку

```
void reverseStr(string& str) //tCmplx: O(N) //Space: O(1)
{
  int n = str.length(); // Swap character starting from two corners
  for (int i = 0; i < n / 2; i++) {
    std::swap(str[i], str[(n - 1) - i]);
    //or
    //char temp = str[i];
    //str[i] = str[ (len-1) - i];
    //str[(len-1) - i] = temp;
  }
}
//#include <bits/stdc++.h> and std::reverse(str.begin(), str.end()); // Reverse str[begin..end]
// OR  Use of reverse iterators => string rev = string(str.Rbegin(), str.Rend());
```

### 3.1 Возведение в степень - Возведение в степень (Итеративно) (Доп.)
A: IN 2^4 OUT 16 (2x2x2x2)

```
int my_pow(int x, unsigned int power) // num^pow
{
  if (power == 0) return 1;
  if (power == 1) return x;
  int n = 15;
  while ((power <<= 1) >= 0) n--;
  long tmp = x;
  while (--n > 0)
    tmp = tmp * tmp * (((power <<= 1) < 0) ? x : 1);
  return tmp;
}
```

### 3.2 Возведение в степень - Возведение в степень (Итеративно) (без учёта отрицательных тепеней)
A: IN 2^4 OUT 16 (2x2x2x2)

```
int my_pow(int n, unsigned int p) // num^pow
{
  int c = 1;
  for(int i=0; i<p; i++)
    c = n * c;
	
  return c;
}
```

### 3.3 Возведение в степень - Возведение в степень (Итеративно) (с учётом отрицательных степеней)
A: IN 2^4 OUT 16 (2x2x2x2)

A: IN 2^-2 OUT 0 (1/4 = 1/(2x2))
```
int my_pow(int n, int p) // num^pow
{
  if(p<0) // 2^(-2) = 1/(2^2)
    p=(-1)*p;
   
  int c = 1;
  for(int i=0; i<p; i++)
  {
    c = n * c;
  }

  if(p<0) // 2^(-2) = 1/(2^2)
     c = 1 / c;

  return c;
}
```

### 3.4 Возведение в степень - Возведение в степень (Улучшенный по скорости)
A: IN 2^6 OUT 64 (2x2x2x2x2x2)

A: IN 2^5 OUT 32 (2x2x2x2x2)
```
int my_pow(int n, int p) // num^pow
{
  if(p<0) // 2^(-2) = 1/(2^2)
    p=(-1)*p;

  int c = 1;
  if(p % 2 == 0) // степень чётная => возводим до половины и полвинное умножаем на само себя
  {
    for(int i=0; i<p/2; i++)
    {
      c = n * c;
    }
    c = c * c;
  }
  else // степень не чётная
  {
    for(int i=0; i<p; i++)
    {
      c = n * c;
    }
  }

  if(p<0) // 2^(-2) = 1/(2^2)
     c = 1 / c;

  return c;
}
```

### 4.0 Факториал ПРОБЛЕМЫ

Вычисляя фактореал, мы быстро упрёмся в верхнюю границу для int, unsigned int  даже для long long int, то есть наша функция будет иметь оагрниченный набор входных параметров, ибо сам фактореал растёт супербыстро, даже для небольших входных n, пример: 
1. Для integer / unsigned integer –> упрёмся уже в границу при n <= 12
2. Для long long int –> упрёмся в границу при n<=19
Следовательно, для вычисления n!, где n>19, потребуется что-то более сложное, чем простые операции умножения (см. ниже 4.4)

### 4.1 Факториал (Итеративный) A: IN 5 OUT 125 (1x2x3x4x5)
```
unsigned int my_fact(unsigned int n)
{
  // Если по привычке написали int n, забыв про unsigned
  // if(n < 0) return 0; // или ОШИБКА !
  unsigned int res = 1;
  //for(int i = 1; i < (num+1); i++) // works too
  for  (int i = 2; i <= n; i++)
    res = res * i;
  return res;
}

// tComplx: O(n) | Space O(1)
// (-) works slower then the recursive solution
// (+-) 2 edges of a stick: + more code easier to debug, - more code then in a recursion and requires extra container and or space
// (+) no stackoverflow problem, and Space complexity is O(1)
```

### 4.2 Факториал (Рекурсивный) A: IN 5 OUT 125 (1x2x3x4x5)
```
unsigned int my_fact(unsigned int n)
{
  if (n == 0 || n == 1)
    return 1;
  return (n * factorial(n - 1));
}

// tComplx: O(n) | Space O(n)
// (+) works faster, since we work with the stack memory
// (+-) 2 edges of a stick: + less code / compact solution, - too compact, not perfect for debugging
// (-) stackoverflow problem - since recurs leaves on a stack a return addres (to do the return call) to a function, on each step of the recursion level
```
	
### 4.3 Фактореал - Факториал на Шаблонах

```
template<unsigned N> struct factorial {
 static const unsigned value = N * factorial<N - 1>::value;
};
template<> struct factorial<0> {
 static const unsigned value = 1;
};

// Пример обращения:
const unsigned f5 = factorial<5>::value; // 120
```

### 4.4 Фактореал - Факториал для больших значений / больших чисел n > 19

- https://www.geeksforgeeks.org/factorial-large-number/

```
// C++ program to compute factorial of big numbers
#include <iostream>
using namespace std;
 
#define MAX 500 // Maximum number of digits in output
 
int multiply(int x, int res[], int res_size);
 
// This function finds factorial of large numbers
// and prints them
void factorial(int n)
{
  int res[MAX];
  
  // Initialize result
  res[0] = 1;
  int res_size = 1;
  
  // Apply simple factorial formula n! = 1 * 2 * 3
  // * 4...*n
  for (int x = 2; x <= n; x++)
      res_size = multiply(x, res, res_size);
  
  cout << "Factorial of given number is \n";
  for (int i = res_size - 1; i >= 0; i--)
      cout << res[i];
}
 
// This function multiplies x with the number
// represented by res[].
// res_size is size of res[] or number of digits in the
// number represented by res[]. This function uses simple
// school mathematics for multiplication.
// This function returns the
// new value of res_size
int multiply(int x, int res[], int res_size)
{
  int carry = 0; // Initialize carry
  
  // One by one multiply n with individual digits of res[]
  for (int i = 0; i < res_size; i++) {
    int prod = res[i] * x + carry;
  
    // Store last digit of 'prod' in res[]
    res[i] = prod % 10;
  
    // Put rest in carry
    carry = prod / 10;
  }
  
  // Put carry in res and increase result size
  while (carry) {
    res[res_size] = carry % 10;
    carry = carry / 10;
    res_size++;
  }
  return res_size;
}
 
int main()
{
  factorial(100);
  return 0;
}
```


### 5.1 перевод строки в число =  (1) atio "своими руками"
A: IN строка str="345" OUT в число типа int n=345

```
int myAtio(std::string a = "345")
{
  int c = 0;
  int len = a.size();
  
  for(int i=0; i < len; i++)
  {
    c = c*10;
    switch(a[i])
    {
      case '0': c=c+0; break;
      case '1': c=c+1; break;
      case '2': c=c+2; break;
      case '3': c=c+3; break;
      case '4': c=c+4; break;
      case '5': c=c+5; break;
      case '6': c=c+6; break;
      case '7': c=c+7; break;
      case '8': c=c+8; break;
      case '9': c=c+9; break;
    }
  }
  return c;
}
```
		       
### 5.2 перевод строки в число = atio "своими руками"
A: IN строка str="345" OUT в число типа int n=345

```
int myAtio(std::string a = "345")
{
  int c = 0;
  int len = a.size();
  for(int i=0; i < len; i++)
  {
    c = c * 10;
    c = (a[i] - '0') + c;
  }
  return c;
}
```

### 6. перевод числа в строку
```
1. Time Complexity: O(n) Auxiliary Space: O(n)
 int num = 2016;
 ostringstream str1;
 str1 << num;
 string geek = str1.str();

2. Time Complexity: O(n) Auxiliary Space: O(n)
 int i_val = 20;
 string stri = to_string(i_val);
  
3.
 string strf = boost::lexical_cast<string>(f_val);
```

### 7. Из строки с пробелами удалить все пробелы в строке
```
class DeleteAllSpacesFromString
{
public:
 DeleteAllSpacesFromString();
 ~DeleteAllSpacesFromString();
    
 /// Решение с выделением доп памяти
 std::string deleteAllSpacesFromString(const std::string& s)
 {
   std::string res;
   for (auto symbol: s) {
     if(symbol != ' ') {
       res.push_back(symbol);
     }
   }
   return res;
 }

 /// Решение БЕЗ выделения доп памяти
 void deleteAllSpacesFromStringExtra(std::string& s)
 {
   for (int i = 0; i< s.size(); ++i) {
     if (s[i] == ' ') {
       for (int j = i + 1; j < s.size(); ++j) {
         if (s[j] != ' ') {
           s[i] = s[j];
           s[j] = ' ';
         }
       }
     }
   }
 }
};
	
```

### 8. Удалить все подстроки в строке

```
#include <string>
#include <iostream>

using namespace std;

template<typename T>
void removeSubstrs
(
  basic_string<T> &s,
  const basic_string<T>& psub
)
{
  size_t n = psub.length();

  for
  (
    size_t i = s.find(psub);
    i != basic_string<T>::npos;
    i = s.find(psub)
  )
  {
    s.erase(i, n);
  }
}

int main()
{
   string s = "One fish, two fish, red fish, blue fish";
   string p = "fish";
   string sorig(s);
   removeSubstrs(s, p);
   cout << s << '\n';
}
	
```

### ПРОВЕРИТЬ
```
int binary_find(int n, vector<int>& v, long x)
{
 int m =0, left = 0, right = n-1;
 while (true)
 {
  if (left > right) return (-1); // значение не найдено
  m = left + (right - left) / 2;
  if (v[m] < x) left  = m + 1;
  if (v[m] > x) right = m - 1;
  if (v[m] == x) return m;
 }
}
```

### 9. Алгоритм Бинарного поиска / Бинарный/Двоичный поиск С++
### 9. Recursive implementation of Binary Search tCmplx: O(log n) ; Space: O(1)
```
//int binarySearch(int arr[], int L, int r, int x)

int binarySearch(vector<int>& arr, int L, int r, int x)
{
  if (r >= L) {
    int mid = L + ((r - L) / 2);
    if (arr[mid] == x) // If x is present at the middle itself
      return mid;
    // X < than mid, => it can only be present in left subarray
    if (arr[mid] > x)
      return binarySearch(arr,       L, mid - 1, x);
    return binarySearch(  arr, mid + 1,       r, x);
    // Else the element can only be present in right subarray
  }
  return -1; // We reach here when element is not present in array
}
 
int main(void)
{
  //int arr[] = { 2, 3, 4, 10, 40 };
  //int n = sizeof(arr) / sizeof(arr[0]);
  
  vector<int> arr = { 2, 3, 4, 10, 40};
  int x = 10;
  int result = binarySearch(v, 0, arr.size() - 1, x);
}
```

### 9. Алгоритм Бинарного поиска / Бинарный/Двоичный поиск С++
### 9. Iterative implementation to Binary Search tCmplx: O(log n) ; Space: O(1)
```
int binarySearch(vector<int>& v, int To_Find)
{
  int lo = 0, hi = v.size() - 1;
  int mid;
  // This below check covers all cases , so need to check
  // for mid=lo-(hi-lo)/2
  
  while (hi - lo > 1) { // while(lo<=hi)
    int mid = (hi + lo) / 2;
    
    if (v[mid] < To_Find) {
        lo = mid + 1;
    }
    else {
        hi = mid;
    }
  }
  
  if (v[lo] == To_Find) { cout << "Found" << " At Index " << lo << endl; }
  else if (v[hi] == To_Find) { cout << "Found" << " At Index " << hi << endl; }
  else { cout << "Not Found" << endl; }
}
 
int main()
{
  vector<int> v = { 1, 3, 4, 5, 6 };
  int To_Find = 1;
  binarySearch(v, To_Find);
}
```
	
### 10.1 Сортировка  пузырьком
```
template<typename T>
void bubble_sort(T array[], std::size_t size)
{
 for (std::size_t idx_i = 0; idx_i < size - 1; idx_i++)
 {
  for (std::size_t idx_j = 0; idx_j < size - idx_i - 1; idx_j++)
  {
   if (array[idx_j + 1] < array[idx_j])
   {
    std::swap(array[idx_j], array[idx_j + 1]);
   }
  }
 }
}
```
### 10.2 Сортировка  пузырьком улучшенный
```
void bubbleSort(int* arrayPtr, int arrLen) // сортировка пузырьком
{
  int temp = 0; // временная переменная для хранения элемента массива
  bool exit = false; // булевая переменная для выхода из цикла, если массив отсортирован
 
  while (!exit) // пока массив не отсортирован
  {
    exit = true;
    for (int i = 0; i < (arrLen - 1); i++)  // внутренний цикл
    {
    //сортировка пузырьком по возрастанию - знак >
    //сортировка пузырьком по убыванию - знак <
    if (arrayPtr[i] > arrayPtr[i + 1]) // сравниваем два соседних элемента
    {
     // выполняем перестановку элементов массива
     temp = arrayPtr[i];
     arrayPtr[i] = arrayPtr[i + 1];
     arrayPtr[i + 1] = temp;
     exit = false; // на очередной итерации была произведена перестановка элементов
    }
    }
  }
}
```

### 11. Развернуть linked_list
```
// Link list node
struct Node {
  int data;
  struct Node* next;
  Node(int data)
  {
    this->data = data;
    next = NULL;
  }
};

struct LinkedList {
  Node* head;
  LinkedList() { head = NULL; } 
	
  // Function to reverse the linked list
  void reverse()
  {
    // Initialize current, previous and next pointers
    Node* current = head;
    Node *prev = NULL, *next = NULL;
    while (current != NULL) {
      next = current->next; // Store next
      current->next = prev; // Reverse current node's pointer
      prev = current; // Move pointers one position ahead.
      current = next;
    }
    head = prev;
  }
  
  void push(int data)
  {
    Node* temp = new Node(data);
    temp->next = head;
    head = temp;
  }
};
 
int main()
{
  // Start with the empty list
  LinkedList ll;
  ll.push(20);
}
```

### 12 split / spli_string / split string своими руками
https://www.cppstories.com/2018/07/string-view-perf-followup/

- STD::STRING MY
```
std::vector<std::string> my_split(const std::string& source, const std::string& delimiters = " ") {
  std::size_t prev = 0;
  std::size_t currentPos = 0;
  std::vector<std::string> results;

  while 
  (
    currentPos = source.find_first_of(delimiters, prev)
    != std::string::npos
  )
  {
    if (currentPos > prev) {
      results.push_back(source.substr(prev, currentPos - prev));
    }
    prev = currentPos + 1;
  }
  if (prev < source.length()) {
    results.push_back(source.substr(prev));
  }
  return results;
}
```

- STD::STRING
```
#include <algorithm>
std::vector<std::string> my_str_split
  (const std::string& str, const std::string& delims = " ")
{
  std::vector<std::string> output;
  auto s_begin_or_next = str.cbegin();

  while (s_begin_or_next != str.cend())
  {
    auto s_delim_pos = std::find_first_of
    (
      s_begin_or_next, str.cend(),
      delims.cbegin(), delims.cend()
    );

    if (s_begin_or_next != s_delim_pos)
    	output.emplace_back(s_begin_or_next, s_delim_pos);
    if (s_delim_pos == str.cend())
    	break;
    //s_begin_or_next = std::next(s_delim_pos);
    s_begin_or_next = ++s_delim_pos;
  }
  return output;
}
```

- STD::STRING_VIEW
```
std::vector<std::string_view>
split_sv(std::string_view strv, std::string_view delims = " ")
{
  std::vector<std::string_view> output;
  size_t first = 0;

  while (first < strv.size())
  {
    const auto second = strv.find_first_of(delims, first);
    if (first != second)
        output.emplace_back(strv.substr(first, second-first));
    if (second == std::string_view::npos)
        break;
    first = second + 1;
  }
  return output;
}
```

https://mainfunda.com/unique_ptr-auto_ptr/

### unique_ptr своими руками

```
#include <iostream>  //main header
using namespace std; //namespace

template<typename T>
class the_unique_ptr  //smart pointers implementation
{
    T* up_data = nullptr;

    public:
    the_unique_ptr(T* p) 
    {
        up_data = p;
    }
    
    the_unique_ptr(const the_unique_ptr& up_rhs) = delete;
    
    the_unique_ptr(the_unique_ptr&& up_rhs)
    {
        up_data = std::move(up_rhs.up_data);
        up_rhs.up_data = NULL;
    }
    the_unique_ptr& operator=
            (const the_unique_ptr& up_rhs) = delete;

    the_unique_ptr& operator=
            (the_unique_ptr&& up_rhs)
    {
        up_data = std::move(up_rhs.up_data);

        up_rhs.up_data = NULL;
        return (*this);
    }
    T& operator*()
    {
        return (*up_data);
    }
    T* operator->()
    {
        return up_data;
    }
    ~the_unique_ptr()
    {
        cout << "Automatic clean-up" 
             << endl;
        if(up_data)
            delete up_data;
        up_data = NULL;
    }
};

int main()
{
    the_unique_ptr<int> up1 = new int(2);
    the_unique_ptr<int> up2 = std::move(up1);
    *up2 = 4;
    cout << *up2 << endl;

    the_unique_ptr<std::string> up3 = 
                       new std::string("mainfunda");
    the_unique_ptr<std::string> up4 = std::move(up3);
    cout << *up4 << endl;
    cout << up4->size() << endl;

    return 0;
}
```

https://medium.com/analytics-vidhya/c-shared-ptr-and-how-to-write-your-own-d0d385c118ad

https://www.geeksforgeeks.org/how-to-implement-user-defined-shared-pointers-in-c/

https://gist.github.com/jakab922/e3b906e7438be7851109c08364696585

### shared_ptr своими руками ЧАСТЬ 1

http://artemy-kolesnikov.blogspot.com/2014/11/weakptr.html
	
```
#include <bits/stdc++.h>
using namespace std;

template <typename T>
class my_shared_ptr {
    T *data;
    int *ref_count;

    void _possibly_destroy() {
        if (*ref_count == 0) {
            delete data;
            delete ref_count;
            data = nullptr;
            ref_count = nullptr;
        }
    }

   public:
    my_shared_ptr() : data{nullptr}, ref_count{nullptr} {}

    my_shared_ptr(T *_data) : data{_data}, ref_count{new int{1}} {}

    // copy constructor
    my_shared_ptr(const my_shared_ptr<T> &other) {
        data = other.data;
        ref_count = other.ref_count;
        (*ref_count)++;
    }

    // copy assignment operator
    my_shared_ptr<T> &operator=(const my_shared_ptr<T> &other) {
        if (other.data == data) return *this;
        (*ref_count)--;
        _possibly_destroy();
        data = other.data;
        ref_count = other.ref_count;
        (*ref_count)++;
        return *this;
    }

    //move constructor
    my_shared_ptr(my_shared_ptr<T> &&other) {
        data = other.data;
        ref_count = other.ref_count;
        other.reset();
    }

    // move assignment operator
    my_shared_ptr<T> &operator=(my_shared_ptr<T> &&other) {
        (*ref_count)--;
        _possibly_destroy();
        data = other.data;
        ref_count = other.ref_count;
        other.reset();
        return *this;
    }

    ~my_shared_ptr() {
        if (ref_count == nullptr) return;  // If it was resetted there isn't much to do here.
        (*ref_count)--;
        _possibly_destroy();
    }

    size_t use_count() const {
        return *ref_count;
    }

    T *get() {
        return data;
    }

    void reset() {
        data = nullptr;
        ref_count = nullptr;
    }
};

int main() {
    cout << "normal constructor" << endl;
    my_shared_ptr<int> one{new int{1}};                      // normal constructor
    cout << "one.use_count(): " << one.use_count() << endl;  // should be 1

    cout << endl
         << "copy constructor" << endl;
    my_shared_ptr<int> other{one};                               // copy constructor;
    cout << "one.use_count(): " << one.use_count() << endl;      // should be 2
    cout << "other.use_count(): " << other.use_count() << endl;  // should also be 2
    assert(one.get() == other.get());                            // They should point to the same object
}
```

### shared_ptr своими руками ЧАСТЬ 2

```

#include <iostream>
using namespace std;
 
class Counter // Class representing a reference counter class
{
public:
  Counter() : m_counter(0){};
  ~Counter() {}
 
  Counter(const Counter&) = delete;
  Counter& operator=(const Counter&) = delete;
 
  void reset() { m_counter = 0; }
  unsigned int get() { return m_counter; }
  // Overload post/pre increment
  void operator++() { m_counter++; }
  void operator++(int) { m_counter++; }
  // Overload post/pre decrement
  void operator--() { m_counter--; }
  void operator--(int) { m_counter--; }
 
  // Overloading << operator
  friend ostream& operator<<
    (ostream& os, const Counter& counter)
  {
    os << "Counter Value : " << counter.m_counter << endl;
      return os;
  }
private:
    unsigned int m_counter{};
};
 
// Class representing a shared pointer
template <typename T>
 
class Shared_ptr
{
public:
    Shared_ptr(): m_ptr{nullptr}, m_counter{nullptr} {}
    Shared_ptr(T *_data): m_ptr{_data}, m_counter{new Counter()}
    {
     (*m_counter)++;
    }
    // Constructor
    explicit Shared_ptr(T* ptr = nullptr)
    {
        m_ptr = ptr;
        m_counter = new Counter();
        (*m_counter)++;
    }
 
    // Copy constructor
    Shared_ptr(Shared_ptr<T>& sp)
    {
        m_ptr = sp.m_ptr;
        m_counter = sp.m_counter;
        (*m_counter)++;
    }
 
    // Reference count
    unsigned int use_count()
    { return m_counter->get(); }
 
    // Get the pointer
    T* get() { return m_ptr; }
    // Overload * operator
    T& operator*() { return *m_ptr; }
    // Overload -> operator
    T* operator->() { return m_ptr; }
   
    // Destructor
    ~Shared_ptr()
    {
        (*m_counter)--;
        if (m_counter->get() == 0)
        {
            delete m_counter;
            delete m_ptr;
        }
    }
 
    friend ostream& operator<<(ostream& os,
                               Shared_ptr<T>& sp)
    {
        os << "Address pointed : " << sp.get() << endl;
        os << *(sp.m_counter) << endl;
        return os;
    }
 
private:
  Counter* m_counter; // Reference counter
  T* m_ptr; // Shared pointer
};
 
int main()
{
    // ptr1 pointing to an integer.
    Shared_ptr<int> ptr1(new int(151));
    cout << "--- Shared pointers ptr1 ---\n";
    *ptr1 = 100;
    cout << " ptr1's value now: " << *ptr1 << endl;
    cout << ptr1;
 
    {
        // ptr2 pointing to same integer
        // which ptr1 is pointing to
        // Shared pointer reference counter
        // should have increased now to 2.
        Shared_ptr<int> ptr2 = ptr1;
        cout << "--- Shared pointers ptr1, ptr2 ---\n";
        cout << ptr1;
        cout << ptr2;
    }
}
```


### shared_ptr weak_ptr & pointers-counter своими руками ЧАСТЬ 3

### sp_counted своими руками ШАГ 1

http://artemy-kolesnikov.blogspot.com/2014/11/weakptr.html

- Код объекта счётчика ссылок теперь будет выглядеть так:
```
template <typename T>
class sp_counted {
public:
  explicit sp_counted(T *p) noexcept
    : shared_count(1),
      weak_count(1),
      ptr(p) {}

  void add_ref() noexcept {
    ++shared_count;
  }

  void release() noexcept {
    if (!--shared_count) {
        // Если последний shared_ptr удалился, удаляем объект
        delete ptr;
        if (!--weak_count) {
            // Если нет слабых ссылок, удаляем объект счётчика
            destroy();
        }
    }
  }

  void add_ref_weak() noexcept {
    ++weak_count;
  }

  void release_weak() noexcept {
    if (!--weak_count) {
        // Если последний weak_ptr удалился, удаляем объект счётчика.
        // Т.к. shared_ptr тоже увеличивает weak_count при создании,
        // нет необходимости проверять значение shared_count
        destroy();
    }
  }

  size_t use_count() const noexcept {
    return shared_count.load();
  }

  // Попытка увеличить счётчик shared_count из weak_ptr
  // Потокобезопасен, lock-free
  void add_ref_lock() {
    // Сохраняем текущее значение shared_count
    size_t cur_value(shared_count.load());
    do {
        // Если счётчик сильных ссылок равен нулю (т.е. нет больше живых shared_ptr),
        // то новый shared_ptr создавать не из чего.
        if (!cur_value) {
            throw std::bad_weak_ptr();
        }
      // Пытаемся увеличить счётчик shared_count на единицу
      // Если в промежутке между сохранением shared_count в cur_value, shared_count изменился,
      // то операция compare_exchange_weak вернёт false, запишет новое значение shared_count в cur_value,
      // и цикл повторится
    } while (shared_count.compare_exchange_weak(cur_value, cur_value + 1));
  }
private:
  void destroy() noexcept {
      delete this;
  }
private:
  // Счётчик ссылок shared_ptr
  std::atomic<size_t> shared_count;
  // Счётчик ссылок weak_ptr
  std::atomic<size_t> weak_count;
  T *ptr;
};
```
Вопрос: почему вызов add_ref_lock безопасен? Ведь в другом потоке shared_ptr в деструкторе может удалить объект sp_counted, и обращение к переменной shared_count будет некорректным.
Ответ: потому что деструктор shared_ptr не удалит объект sp_counted, т.к. есть живая слабая ссылка (weak_count != 0).

### weak_ptr своими руками ШАГ 2
```
template <typename T>
class weak_ptr {
 friend class shared_ptr<T>;
public:
  weak_ptr() noexcept : ptr(nullptr), counted(nullptr) {}

  weak_ptr(const weak_ptr &other) noexcept : ptr(other.ptr), counted(other.counted) {
      add_ref_weak();
  }
  weak_ptr(const shared_ptr<T> &p) noexcept : ptr(p.ptr), counted(p.counted) {
      add_ref_weak();
  }
  weak_ptr &operator=(const weak_ptr &other) noexcept {
      release_weak();

      ptr = other.ptr;
      counted = other.counted;

      add_ref_weak();

      return *this;
  }
  weak_ptr &operator=(const shared_ptr<T> &p) {
      release_weak();

      ptr = p.ptr;
      counted = p.counted;

      add_ref_weak();

      return *this;
  }
  // Пытаемся сделать shared_ptr. Для этого вызывается конструктор shared_ptr(const weak_ptr &amp;);
  // В случае невозможности создать shared_ptr возвращается пустой объект
  shared_ptr<T> lock() noexcept {
      try {
          return shared_ptr<T>(*this);
      } catch (const std::bad_weak_ptr &) {
          return shared_ptr<T>();
      }
  }
  size_t use_count() const noexcept {
      return counted != nullptr ? counted->use_count() : 0;
  }
private:
  void add_ref_weak() noexcept {
      if (counted) {
          counted->add_ref_weak();
      }
  }
  void release_weak() noexcept {
      if (counted) {
          counted->release_weak();
      }
  }
private:
  T *ptr;
  sp_counted<T> *counted;
};
```

### shared_ptr своими руками ШАГ 3
	
В код shared_ptr добавляется конструктор shared_ptr(const weak_ptr &p):
	
```
template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr {
  friend class weak_ptr<T>;
public:
  shared_ptr() noexcept : ptr(nullptr), counted(nullptr) {}

  // excaption safe конструктор
  explicit shared_ptr(T *p) {
      std::unique_ptr<T> holder(p);
      // new может кинуть исключение, и если p не передать в unique_ptr,
      // память под p потеряется
      counted = new sp_counted<T>(holder.get());
      ptr = holder.release();
  }
  ~shared_ptr() noexcept {
      release();
  }
  shared_ptr(const shared_ptr<T> &other) noexcept : ptr(other.ptr), counted(other.counted) {
      add_ref();
  }
  shared_ptr(const weak_ptr<T> &p) : ptr(p.ptr), counted(p.counted) {
      if (counted) {
          // Пытаемся увеличить счётчик ссылок объекта
          // В случае неудачи сгенерируется исключение std::bad_weak_ptr()
          counted->add_ref_lock();
      } else {
          throw std::bad_weak_ptr();
      }
  }
  shared_ptr &operator=(const shared_ptr<T> &other) noexcept {
      // Освобождаем владение предыдущим указателем
      release();

      // Выполняем присваивание
      ptr = other.ptr;
      counted = other.counted;

      // Устанавливаем владение новым указателем
      add_ref();

      // Ура! Я не забыл вернуть *this!
      return *this;
  }
  T *get() const noexcept {
      return ptr;
  }
  size_t use_count() const noexcept {
      return counted != nullptr ? counted->use_count() : 0;
  }
private:
  void add_ref() noexcept {
      if (counted) {
          counted->add_ref();
      }
  }
  void release() noexcept {
      if (counted) {
          counted->release();
      }
  }
private:
  T *ptr;
  sp_counted<T> *counted;
};
```
	
А что с потокобезопасностью?
Гарантии потокобезопасности такие же как и у shared_ptr. Но на всякий случай повторю: любая операция с weak_ptr потокобезопасна, если каждый поток хранит копию weak_ptr. При этом каждая копия может хранить сслыку на один shared_ptr. Хранить ссылки на один weak_ptr в разных потоках непотокобезопасно. 


===================================================================
	
# Паттерны проектирования паттерны проектирования Паттерны Проектирования	
# PATTERN PATTERNS ШАБЛОНЫ ПРОЕКТИРОВАНИЯ СИСТЕМНЫЙ ДИЗАЙН ПАТТЕРНЫ


### https://intuit.ru/studies/courses/32/32/lecture/1026?page=2

| №  | Название паттерна          | Перевод                 | Назначение паттерна
|----|----------------------------|-------------------------|---------------------------------------------------------------------------------------------------------------------------------------
| 1  | Abstract Factory           | Абстрактная фабрика     | Предоставляет интерфейс для создания множества связанных между собой или независимых объектов, конкретные классы которых неизвестны.
| 2  | Adapter(Wrapper)           | Адаптер (Обертка)       | Преобразует существующий интерфейс класса в другой интерфейс, который понятен клиентам. При этом обеспечивает совместную работу классов, невозможную без данного паттерна из-за несовместимости интерфейсов.
| 3  | Bridge                     | Мост                    | Отделяет абстракцию класса от его реализации, благодаря чему появляется возможность независимо изменять то и другое.
| 4  | Builder                    | Строитель               | Отделяет создание сложного объекта от его представления, позволяя использовать один и тот же процесс разработки для создания различных представлений.
| 5  | Chain of Responsibility    | Цепочка обязанностей    | Позволяет избежать жесткой зависимости отправителя запроса от его получателя, при этом объекты-получатели связываются в цепочку, а запрос передается по цепочке, пока какой-то объект его не обработает.
| 6  | Command                    | Команда                 | Инкапсулирует запрос в виде объекта, обеспечивая параметризацию клиентов типом запроса, установление очередности запросов, протоколирование запросов и отмену выполнения операций.
| 7  | Composite                  | Компоновщик             | Группирует объекты в иерархические структуры для представления отношений типа "часть-целое", что позволяет клиентам работать с единичными объектами так же, как с группами объектов.
| 8  | Decorator                  | Декоратор               | Применяется для расширения имеющейся функциональности и является альтернативой порождению подклассов на основе динамического назначения объектам новых операций.
| 9  | Facade                     | Фасад                   | Предоставляет единый интерфейс к множеству операций или интерфейсов в системе на основе унифицированного интерфейса для облегчения работы с системой.
| 10 | Factory Method             | Фабричный метод         | Определяет интерфейс для разработки объектов, при этом объекты данного класса могут быть созданы его подклассами.
| 11 | Flyweight                  | Приспособленец          | Использует принцип разделения для эффективной поддержки большого числа мелких объектов.
| 12 | Interpreter                | Интерпретатор           | Для заданного языка определяет представление его грамматики на основе интерпретатора предложений языка, использующего это представление.
| 13 | Iterator                   | Итератор                | Дает возможность последовательно перебрать все элементы составного объекта, не раскрывая его внутреннего представления.
| 14 | Mediator                   | Посредник               | Определяет объект, в котором инкапсулировано знание о том, как взаимодействуют объекты из некоторого множества. Способствует уменьшению числа связей между объектами, позволяя им работать без явных ссылок друг на друга и независимо изменять схему взаимодействия.
| 15 | Memento                    | Хранитель               | Дает возможность получить и сохранить во внешней памяти внутреннее состояние объекта, чтобы позже объект можно было восстановить точно в таком же состоянии, не нарушая принципа инкапсуляции.
| 16 | Observer                   | Наблюдатель             | Специфицирует зависимость типа -один ко многим- между различными объектами, так что при изменении состояния одного объекта все зависящие от него получают извещение и автоматически обновляются.
| 17 | Prototype                  | Прототип                | Описывает виды создаваемых объектов с помощью прототипа, что позволяет создавать новые объекты путем копирования этого прототипа.
| 18 | Proxy                      | Заместитель         	| Подменяет выбранный объект другим объектом для управления контроля доступа к исходному объекту.
| 19 | Singleton                  | Одиночка                | Для выбранного класса обеспечивает выполнение требования единственности экземпляра и предоставления к нему полного доступа.
| 20 | State                      | Состояние               | Позволяет выбранному объекту варьировать свое поведение при изменении внутреннего состояния. При этом создается впечатление, что изменился класс объекта.
| 21 | Strategy                   | Стратегия               | Определяет множество алгоритмов, инкапсулируя их все и позволяя подставлять один вместо другого. При этом можно изменять алгоритм независимо от клиента, который им пользуется.
| 22 | Template Method            | Шаблонный метод         | Определяет структуру алгоритма, перераспределяя ответственность за некоторые его шаги на подклассы. При этом подклассы могут переопределять шаги алгоритма, не меняя его общей структуры.
| 23 | Visitor                    | Посетитель              | Позволяет определить новую операцию, не меняя описаний классов, у объектов которых она вызывается.



https://en.wikipedia.org/wiki/Double-checked_locking

https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
	
С С++11 проблемы больше для поддержки потоков ничего и не надо. Но до полной его поддержки всеми компиляторами надо ещё дожить.
```	
class S
{
public:
  static S& getInstance()
  {
    static S instance;
    return instance;
  }
private:
  S() {}                    
  S(S const&);             
  void operator=(S const&);
public:
    S(S const&)               = delete;
    void operator=(S const&)  = delete;
};
```


```
class S
{
  public:
    static S& getInstance()
    {
      static S    instance; // Guaranteed to be destroyed.
                            // Instantiated on first use.
      return instance;
    }
  private:
    S() {}                    // Constructor? (the {} brackets) are needed here.

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    S(S const&);              // Don't Implement
    void operator=(S const&); // Don't implement

    // C++ 11
    // =======
    // We can use the better technique of deleting the methods
    // we don't want.
  public:
    S(S const&)               = delete;
    void operator=(S const&)  = delete;

    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //       before deleted status
};	
```
	
### Q: Синглтон Класс (небезопасный в многопоточном окружении до С++11)
```
// Singleton.h
class Singleton
{
private:
  static Singleton * p_instance;
  // Конструкторы и оператор присваивания недоступны клиентам
  Singleton() { }
  Singleton( const Singleton& );  
  Singleton& operator=( Singleton& );
public:
  static Singleton * getInstance() {
    if(!p_instance)           
      p_instance = new Singleton();
    return p_instance;
  }
};
```

### Q: Синглтон Мейерса / Майерса (небезопасный в многопоточном окружении  до С++11) КОД 
```
// Singleton.h
class Singleton
{
private: 
  Singleton() {}
  Singleton( const Singleton&);  
  Singleton& operator=( Singleton& );
public:
  static Singleton& getInstance() {
    static Singleton  instance;
    return instance;
  }    
}; 
```	

### Q: Синглтон Дейкстры (использующий метода "Double-Checked Locking" для threadsafity) 
Ранее проблему решали с использованием метода "Double-Checked Locking" / "Double-Checked Locking Pattern" / Блокировка с двойной проверкой, хоть он и был более затратен.
По причине возникновения такого решения, синглтон с использованием "Double-Checked Locking" назвали Синглтон Дейкстры.	

```
#include <atomic>
#include <mutex>

class Singleton {
public:
 static Singleton* GetInstance() 
 {
   Singleton* p = s_instance.load(std::memory_order_acquire);
   if (p == nullptr) { // 1st check
     std::lock_guard<std::mutex> lock(s_mutex);
     p = s_instance.load(std::memory_order_relaxed);
     if (p == nullptr) { // 2nd (double) check
       p = new Singleton();
       s_instance.store(p, std::memory_order_release);
     }
   }
   return p;
 }	

private:
 Singleton() = default;

 static std::atomic<Singleton*> s_instance;
 static std::mutex s_mutex;
};

```	

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
```	
_______________________->update() ------------------------------------ ObservA()
[Subject->Subjects changes]->update()] ObserverBase with notify() -> ObservB()
_______________________->update() --------------------------------------> ObservC()
```
- **Command** - Turns a request into a stand-alone object that contains all information about the request. This transformation lets you pass requests as a method arguments, delay or queue a request's execution, and support undoable operations.
- **Iterator** - Lets you traverse elements of a collection without exposing its underlying representation (list, stack, tree, etc.).
- **Proxy** - Lets you provide a substitute or placeholder for another object. A proxy contr- ols access to the original object, allowing you to perform something either before or after the request gets through to the original object.- **

- **Adapter** - Allows objects with incompatible interfaces to collaborate.
- **Composite** - Lets you compose objects into tree structures and then work with these structures as if they were individual objects.
- **Prototype** - Lets you copy existing objects without making your code dependent on their classes.
- **Decorator** - Lets you attach new behaviors to objects by placing these objects inside special wrapper objects that contain the behaviors.
	
	
===================================================================
	
	
# HISTORY	
	
Was das fachliche anbetrifft.

In der IT brounche habe ich als 
- Windows SystemAdministartor zu arbeiten. Meine Arbeit bestand darin dass ich Oper.Systeme auf die PCs installiert habe, Drucker und andere Geräte angegschlossen, Einrichtung lokaler Netze, etwas ingenior Arbeit. Ich habe mich hier mit BorlandC++ Builder begegnet. War kein richtiges programieren, aber die erste Begegnung mit c++ Sprache und mit IDE.

- Dann im Dritten StudienJahr hatten wir ein Betriebspraktikum bei Kaspersky. Dort leitete man uns in das Project ein, man hat unz gezeigt wie VierenAnalytiker arbeiten, wie Sie die meiste Zeit das untershiedlichste code sehen, VisualBasic code, C++, JavaScript und andere, wie man den Reverce Enegeniering des Codes macht. 

Ich habe rheines C code für einige Teile des Analyseszstems geschrieben, Suchalgorythmen analisiert und sehr viel debugging von code gemacht um Patterns zu entdecken. 

Ich konnte nich angestellt werden, da sie nur nach Seniour SWE suchten, nur Seniour Positionen waren in der Firma offen, und ich hatte noch nicht genug ERFAHRUNG.
SW Enwicklung hat mir noch ab der Uni gefallen. Dort war beides interssant – sowohl das obeflachlicke auf der Client seite alsauch die backend seite der programmierung. Der fakt das man mit code etwas erschaffen kann und das es funkzioniert und alles was man dafür braucht ist – nur code zu schreiben. Obwohl wir in dem 1-sten StudienJar nur rheines C, Pascal und Visual Basic hatten, trotzdem, als man mir gezeigt hat wie man diese Basis Kentnisse in der Realität nutzt und das man damit große software produkte ertellen kann, ich wollte nur mehr und tiefer mich damit beschäftigen und weiter als SWE zu arbeiten.

- Als SW Enwitckler habe in der Firma Symbol Designe (Task in Task out, Kanban)
Windows Client Server Application – Es waren mehrere SCADA-Systeme für Fixierung von Erdölleck auf der Erdölleitung.  Die mit C++ entwickelt worden waren. Wieso – weil man jeden Protokol unterschtützen konnte, und Maximale flexibilität.  Viel GUI-Programmierung (MFC classen, BCGP bibl) und etwas Multi-Threading mit standarten STL-Funktionalität und deren Objekte std::condition_variable, std::thread, deque, algorithm Transport der Daten zwischen dem Server und den Geräten (Cleine SCADA Densitometer, Controller, Druckmäsgeraten) war OPC und COM/DCOM um mit C# server von ein par Firmen extra zu kommunizieren.  Die Firma war zu klein, nach drei Jahren habe ich gespürt, dass ich nicht wachse, und ich wollte mich weiter entwickeln und mehr Verdienen

- C++ Softwareentwickler Encore GmbH, Moskau. (Kleines Team aus 2 Entwickler erste begegnung mit Scrum)
- Ich habe mich der Entwicklung eines Benutzer Authentifizierungssystem für Windows und Astralinux (Credential Provider, dadurch erlaubt Windows den Winlogon zu customisieren) angeschlossen. Ich habe extra den Teil für das einloggen mit CryptoToken geschrieben. Viel WINAPI + CryptoAPI + Nur BackendSeite. Dazu noch extra Entwicklung von Bibliotheken und Windows-Services die private Benutzer-Informationen spechern und bearbeiten. (Token Zugriff und etc.)
- Entwicklung von Integritätskontrollsystem von Dateien (Ordnern) und Windows-Registrierungsdatenbank. In diesem Projekt habe ich persönlich - GUI auf QT entwickelt und den Teil für Integritätskontrolle von Registrierungsdatenbank auf mich genommen

Ich habe kurz auf dieser Stelle gerabeitet, weil die Firma sich geschlossen hat. Es hatte mehr mit der Arbeit der Manager und einer nicht richtigen Posizionierung der Firma auf dem Markt, und die Firma ist Pleite gegangen. Und ich musste meine Arbeit wechseln.

- C++ Leading Developer RGS GROUP OAG, Moskau – zu einfache Arbeit, zu alte Technologien, und kein Wachstum als Spezialist ... CRM - customer relationship management - Nicht das ich von jeder Arbeit weggeehe wo es mir zu einfach alles scheint. Ich habe 2 bis 3 Wochen nicht gearbeitet und keine Aufgaben gebat. Der Grund dafür war der Waterfall-Prinziep. Es was ein altes Produrk, einige APIS waren noch in 1995 geschrieben. Es war eiene Große Firma, und sie stellte BtoB produkte, also Ihre Kunden waren kleinere Versicherungs-Firmen. Meistens machte der Kunde einfach 3 – 10 screenshots von unterschiedlichster Software, hat die Technische Aufgabe beschrieben und, danach haben die Analytiker von dem Kunden extra Information über das Produkt gesammelt. Am nächsten Schritt machten vor ein Entwurf der DatenBank (zusammen mit den DatenBank Spetialisten), oder wir habe besprochen wie wir das in das schon existierte szstem integrieren konnten. SoftwareDesign - GUI-Makett wurde entworfen und die Business logig beschrieben. Ich bekam (WATERFALL tief durchVerabeitete TA) eine 200 seitige Technische Aufgabe, und man musste es innerhalb von 1 oder 2 Monate alles fertig stellen.
	
ERP/CRM/CAD/CAM-System vorbestimmt für Berechnungen von Versicherungslebensmitteln. Entwicklung von Backend in C++ und GUI mit MFC-Framework in MS Visual Studio 2013 (neue Apps mit Qt (QML) in QtCreator)
Ein bischen habe ich dort auch mit Neuronetzen gearbeitet, aber diese Abteilung der Firma hat sich zu spät organisiert und ich hatte zu diesem Zeitpunkt schon einen JobOffer von der TochterFirma der Russischen Bahn auf der Hand

Es waren etwas zu kurze Stationen,  der Grund dafür lag daran das ich 
- 01/2018 – bis heute C++/Qt Leading Software Engineer Locotech-Signal Moskau	TochtorFirma der Russischen Bahn
Client- und Serverseiten von SCADA für Eisenbahnverkehrsysteme.

1 Jahr habe ich als normaler SoftwareEngenier gearbeitet und dann Habe ich ein TeamLead 4 Entwicklern geleitet. Ich war ScrumMaster, habe scrum Meetings geführt. Das übliche halt, Iteration gestartet, Große Aufgabe besprochen, sie in kleinere Aufgaben aufgeteilt - Dekomposition gemacht, nachdem ware Aufgaben auch dem Scrum-Board, Arbeits-zeiten für jede aufgabe geplant und fixiert and Iteration startete. Hier arbeiteten wir nach einem normalen Scrum, mit allen Teilnehmer des Entwicklungsprozess: Analytiker, UI/UI-Designer, SoftwareEntwickler, Tester

1. ClientServerApp 2. StationDesigner – CAD-System 3. Logs Player 

Eisenbahnverkehrsysteme. GUI Implementierung mit Qt Widgets, tiefe benutzerdefinierte Implementierung. Entwicklung von Visualisation des Eisenbahnverkehrs, als ein QtQuickView-Komponent mit standarten und selbst entwickelten benutzerdefinierten QtQuick-Controls (viel QML). 80% von allen Projekten der Firma sind mit Qt-Framework geschrieben (nicht nur Qt GUI-Classen sondern auch Qt-Container / Threading Classen / QSqlDatabase / QModbus und andere.

Client - QMLScene in der Mitte mit Widgets umgeben - Treeviews mit statinen links, Alarame Warnungen Informations Messeges unten in ter TabellenViw, Rects die wichtingsten Knöpfe,

- Senior Software Engineer at "IPG Photonics",
Reimplemented old Singleton-based application solution by adding more suitable application and communication layers. 
Singleton – extra problem – it can become a SUPERCLASS, that knows everything about everyone and every software part and software layer can have access to any variable or object that’s inside the Singleton (Laser-class).

- Singleton –  We can have any type of problems – starting from INCONSISTANCY OF VARIABLE STATES we read a buffered value, that has already changed it’s state on the device, finishing with – if it’s an shared resource and accessed from MULTIPLE THREADS at the same time, and it has a synchronization object we can get a deadlock
Singleton – having only 1 instance of an object. – it’s OK if it’s not shared it’s perfect to get access from allover the place to this entity. A Singleton encapsulates a unique resource and makes it readily available throughout the application.
- Singleton – shared resource [stored data] multiple objects must have access to that data, here we start to have race conditions (here we can use mutexes, cond-vars, double check pattern approach)
- SOLUTION WAS – we just have sent the commands not in Asyncronious manner but in Syncronious manner. There for we could right away have time to wait for the answer, if we did not got it => means the device, board, firmware, or something else was broken either way and we had to shut down the system. The syncronious manar also gave us the possibility easy to send multiple commands always in a correct and for sure known order. And we have sent the commands not from the Superclass that contained each and every state, but we just called it inside of thar particular place, where these commands were really needed. If we had to know anything else abot multiple systemparts or their states, we always could pass a pointer of that instance, we could collect multiple pointers in a deccisionManagerClass,  use a proxy class ETZETERA it was up to the situation

Developed a line following algorithm for a laser knife which uses CAN Bus to communicate with camera and the laser.
The ALGO had 3 steps:
1) First there was a laser pointer, and it pointed out the way of the laser knife. We pressed the button and the system SAVED the “way” for the knife. Then we have chosen Frequency, Power and Pulse Energy for the laser, and how deep should the knife go in. 
2) Then The one thread got the information about the knife positions and compared it to the positions of the way, that the knife should go, following the vector that was calculated as the nearest waypoint between 2 points
WE had only a CONDITION_VARIABLE to notify both threads that something went wrong.
3) The laser pointer in the other thread was watching for the organ, and was watching if there will be any unexpected movement during the procedure.
4) These 2 threads did not have any shared objects or shared memory, so there was not that much of concurrency stuff to be handled.

CONS in general: OVERENGENEERING

# EVERY DAY

- 80
https://www.geeksforgeeks.org/top-10-algorithms-in-interview-questions/
- 7
https://medium.com/pythoneers/7-must-know-algorithms-for-your-next-coding-interview-26252748b895


### class methods
То есть объявление class A {} может быть эквивалентно следующему:
```
  A() {} 
  ~A() {} 
  A(const A& that) {}
  A& operator=(const A& that) {}
  A(A&& that) {} // C++11
  A& operator=(A&& that) {} // C++11
};
```

### Singleton OLD

```
class Singleton {  
public:
  static Singleton* getInstance() 
  {
    if (theInstance == nullptr )
    { theInstance = new Singleton(); }
    return theInstance;
  }
 private:    
   static Singleton* theInstance;
   
   Singleton() { }
};

Singleton* Singleton::theInstance = nullptr;
```

### Singleton MAYERS

```
// initialized the first time control passes to 
// this declaration. Ignored on subsequent calls.
// Since C++11, If multiple threads attempt to 
// initialize the same static local variable concurrently,
// the initialization is still guaranteed to occur 
// exactly once. see 
// https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables

class Singleton
{
public:
  static Singleton& instance()
  {
    static Singleton s; 
    return s;
  }
private:
  Singleton() {}
  Singleton(const Singleton &) = delete;
  Singleton& operator= (const Singleton &) = delete;
};
```

### Custom String mystring implementation

```
#include <cstring>
#include <iostream>
using namespace std;
 
class Mystring {
  char* str; // Init char array
public:
  Mystring(); // No arguments Ctor
  Mystring(char* val); // Ctor with 1 arguments
  Mystring(const Mystring& source); // Copy Ctor
  Mystring(Mystring&& source);
  ~Mystring() { delete str; }
};
 
// Ctor with no arguments
Mystring::Mystring() : str{nullptr} {
  str = new char[1];
  str[0] = '\0';
}

Mystring::Mystring(char* val) { // Ctor with one arguments
  if (val == nullptr) {
    str = new char[1];
    str[0] = '\0';
  }
  else {
    str = new char[strlen(val) + 1];
    strcpy(str, val); // Copy character of val[] using strcpy
    str[strlen(val)] = '\0';
  }
}
 
Mystring::Mystring(const Mystring& source) { // Copy Ctor
  str = new char[strlen(source.str) + 1];
  strcpy(str, source.str);
  str[strlen(source.str)] = '\0';
}
 
Mystring::Mystring(Mystring&& source) { // Move Constructor
  str = source.str;
  source.str = nullptr;
}
 
int main()
{
  Mystring a; // Constructor with no arguments
  char temp[] = "Hello"; // Convert string literal to char array
  Mystring b{ temp }; // Ctor with one argument
  Mystring c{ a }; // Copy ctor
  char temp1[] = "World";
  // One arg constructor called,
  // then the move constructor
  Mystring d{ Mystring{ temp } };
  
  return 0;
}
```

### Custom String mystring with Iterators implementation
https://gist.github.com/philipheimboeck/099e540d800063e3e6ec

```

#include <string.h>
#include <iostream>

template<typename T>
class It {
private:
  T* data;
public:
  It(T* data)
  {	this->data = data; }
  T &operator*()
  { return *data; }
  bool operator==(const It<T> &a)
  { return data == a.data; }
  bool operator!=(const It<T> &a)
  { return data != a.data; }
  It<T> &operator++() {
    data++;
    return *this;
  }
};

class String {
  // Friend to implement the ostream operator <<
  friend std::ostream &operator<<(std::ostream &iostream, const MyString::String &string);
private:
  char* data;
  size_t size;
public:
  String(const char* data);
  String(size_t init_size = 20);
  String(const String &string);
  ~String();
  char* getText();
  void setText(const char* text);
  size_t getLength() const; // length (\0 is end)
  
  //
  // Set the length of the string
  // If the new length is smaller than its data, everything beyond will be cur
  // If the new length is bigger, all elements will be initialized with \0
  // 
  void setLength(size_t size);
  void add(const String &text);
  char get(size_t pos) const;
  bool compare(const String &string) const;
  // Operators
  char operator[](size_t pos);
  // Iterator
  It<char> begin() {
    It<char> it(data);
    return it;
  }
  It<char> end()
  {
    It<char> it(data + size);
    return it;
  }
};

// Free operator methods
String operator+(const String &string1, const String &string2);
bool operator==(const String &string1, const String &string2);
std::ostream &operator<<(std::ostream &iostream, const MyString::String &string);

// Ctor with empty initialization
String::String(size_t init_size) {
  this->data = new char[init_size];
  this->size = init_size;
  
  if (this->data != NULL) {
    for (size_t i = 0; i < init_size; i++) {
      data[i] = '\0';
    }
  }
}

// Ctor with char*
String::String(const char* data) {
  size_t size = strlen(data);
  
  this->data = new char[size];
  this->size = size;
  
  if (this->data != NULL) {
    strncpy(this->data, data, size);
  }
}

// Copy Ctor
String::String(const String &string) {
  this->size = string.getLength();
  this->data = new char[this->size];
  
  if (this->data != NULL) {
    strncpy(this->data, string.data, this->size);
  }
}

// Destructor
String::~String() {
  if (this->data != NULL)
    delete[] this->data;
}

// Methods
char* String::getText() {
  return data;
}

void String::setText(const char* text) {
  delete[] this->data;
  
  this->size = strlen(text);
  this->data = new char[this->size];
  
  strncpy(this->data, text, this->size);
}

size_t String::getLength() const {
  return strlen(this->data);
}

void String::setLength(size_t size) {
  size_t old_length = this->getLength();
  char* old_data = this->data;
  
  this->size = size;
  this->data = new char[size];
  
  // Copy the characters (or add \0 if there are no characters anymore).
  for (size_t i = 0; i < size; i++) {
    if (i < old_length) {
      this->data[i] = old_data[i];
    }
    else {
      this->data[i] = '\0';
    }
  }

  delete[] old_data; // Destroy the old data
  this->data[size] = '\0'; // Ensure that the last element is \0
}

void String::add(const String &text) {
  size_t new_size = this->size + text.size;
  setLength(new_size);
  
  size_t length = getLength();
  for (size_t i = length; i < new_size; i++) {
    this->data[i] = text.data[i - length];
  }
}
char String::get(size_t pos) const {
  if (pos > getLength()) {
    return '\0';
  }
  return data[pos];
}
bool String::compare(const String &string) const {
  return strcmp(this->data, string.data) == 0;
}
char String::operator[](size_t pos) {
  return this->get(pos);
}

// Free Operator Methods
String operator+(const String &string1, const String &string2) {
  String string = String(string1);
  string.add(string2);
  
  return string;
}
bool operator==(const String &string1, const String &string2) {
  return string1.compare(string2);
}
std::ostream &operator<<(std::ostream &ostream, const MyString::String &string) {
  return (ostream << string.data);
}

```
