# QtSamples
Many small samples of qt projects


http://qt.shoutwiki.com/wiki/Closing_a_Qt_Quick_application_from_QML


### QtSamples
Test


### WIX

https://stackoverflow.com/questions/36671672/how-to-enable-wix-projects-in-visual-studio-2017

https://marketplace.visualstudio.com/items?itemName=RobMensching.WixToolsetVisualStudio2017Extension

https://github.com/wixtoolset/wix3/releases/tag/wix3104rtm


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


## OLD find function version - NO ALGO used


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
