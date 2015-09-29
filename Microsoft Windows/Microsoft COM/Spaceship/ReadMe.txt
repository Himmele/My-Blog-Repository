========================================================================
       MICROSOFT FOUNDATION CLASS BIBLIOTHEK : Spaceship
========================================================================


Diese Spaceship-DLL hat der Klassen-Assistent für Sie erstellt. Die DLL zeigt 
die prinzipielle Anwendung der Microsoft Foundation Classes und dient Ihnen 
als Ausgangspunkt für die Erstellung Ihrer eigenen DLLs.

Diese Datei enthält die Zusammenfassung der Bestandteile aller Dateien, die 
Ihre Spaceship-DLL bilden.

Spaceship.h
	Dies ist die Haupt-Header-Datei, in der die Klasse CSpaceshipApp deklariert ist.

Spaceship.cpp
	Hierbei handelt es sich um die Haupt-Quellcodedatei der DLL. Diese enthält 
	die Klasse CSpaceshipApp.
	Außerdem enthält diese Datei die OLE-Einstiegspunkte, die von Inproc-Servern 
	benötigt werden.

Spaceship.odl
    	Diese Datei enthält den Quellcode der Object Description Language (ODL) für
	die Typbibliothek Ihrer DLL.

Spaceship.rc
	Hierbei handelt es sich um eine Auflistung aller Ressourcen von Microsoft Windows, die 
	vom Programm verwendet werden. Sie enthält die Symbole, Bitmaps und Cursors, die im 
	Unterverzeichnis RES abgelegt sind. Diese Datei lässt sich direkt im Microsoft
	Developer Studio bearbeiten.

res\Spaceship.rc2
    	Diese Datei enthält Ressourcen, die nicht vom Microsoft Developer Studio bearbeitet wurden.
	In diese Datei werden alle Ressourcen abgelegt, die vom Ressourcen-Editor nicht bearbeitet 
	werden können.

Spaceship.odl
    	Diese Datei enthält den Quellcode der Object Description Language (ODL) für die
    	Typbibliothek Ihrer Anwendung.

Spaceship.def
    	Diese Datei enthält Informationen über die DLL, die für das Ausführen unter
	Microsoft Windows erforderlich sind. Es werden Parameter definiert, wie beipielsweise
	der Name und die Beschreibung der DLL. Außerdem werden die von der DLL exportierten
	Funktionen angegeben.

Spaceship.clw
    	Diese Datei enthält Informationen, mit denen der Klassen-Assistent bestehende
    	Klassen bearbeitet oder neue Klassen einfügt. Außerdem verwendet der Klassen-Assistent 
	diese Datei, um Informationen zu speichern, die er für das Erstellen und Bearbeiten von
	Nachrichtentabellen und Dialogfeld-Nachrichtentabellen sowie für das Erstellen von 
	Prototypen vom Member-Funktionen benötigt.

/////////////////////////////////////////////////////////////////////////////
Andere Standarddateien:

StdAfx.h, StdAfx.cpp
    	Mit diesen Dateien werden vorkompilierte Header-Dateien (PCH) mit der Bezeichnung 
	Spaceship.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj
	erstellt.

Resource.h
    	Dies ist die Standard-Header-Datei, die neue Ressourcen-IDs definiert.
    	Microsoft Developer Studio liest und aktualisiert diese Datei.

/////////////////////////////////////////////////////////////////////////////
Weitere Hinweise:

Der Klassen-Assistent fügt "ZU ERLEDIGEN:" im Quellcode ein, um die Stellen anzuzeigen, 
an denen Sie Erweiterungen oder Anpassungen vornehmen können.

/////////////////////////////////////////////////////////////////////////////
