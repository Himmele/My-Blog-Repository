var BaseHref="";
var NoOffFirstLineMenus=5;

var FontFamily="Verdana, Arial, Helvetica, sans-serif";
var FontSize=11;
var FontBold=1;
var FontItalic=0;
var MenuTextCentered="left";
var MenuCentered="left";
var MenuVerticalCentered="top";
var PartOfWindow=0.7;

var LowBgColor="#FF9900";
var HighBgColor="#FBEDBB";
var FontLowColor="Black";
var FontHighColor="Blue";
var BorderColor=LowBgColor;
var HoverBorderColor="#800080";

var BorderWidthMain=2;
var BorderWidthSub=2;
var BorderWidthHover=1;
var BorderBtwnMain=1;
var BorderBtwnSub=1;

var StartTop=0;
var StartLeft=0;
var VerCorrect=0;
var HorCorrect=0;
var LeftPaddng=2;
var TopPaddng=2;
var FirstLineHorizontal=1;
var TargetLoc="MenuPos";

var BgImgLeftOffset=5;
var ScaleMenu=0;
var DistFrmFrameBrdr=0;

var HooverBold=0;
var HooverItalic=0;
var HooverUnderLine=0;
var HooverTextSize=0;
var HooverVariant=0;
var HoverBorder=1;

var DissapearDelay=300;
var UnfoldDelay=200;
var ChildOverlap=0;
var ChildVerticalOverlap=0;
var BuildOnDemand=0;
var BottomUp=0;
var MenuWrap=1;
var RightToLeft=0;
var UnfoldsOnClick=0;
var RememberStatus=0;

var TakeOverBgColor=1;
var FirstLineFrame="";
var SecLineFrame="";
var DocTargetFrame="";
var MenuFramesVertical=1;
var MenuUsesFrames=0;

var MenuSlide="";
var MenuShadow=""; //"progid:DXImageTransform.Microsoft.Shadow(color=#888888, direction=135, strength=4)";
var MenuOpacity=""; //"progid:DXImageTransform.Microsoft.Alpha(opacity=85)";

function BeforeStart(){return}
function AfterBuild(){return}
function BeforeFirstOpen(){return}
function AfterCloseAll(){return}

var Arrws=[BaseHref+"",,,BaseHref+"",,,BaseHref+"",,,BaseHref+"",,];


Menu1=new Array("Help!","/info/faq.asp","",5,18,45,"","","","","","",-1,-1,-1,"","");
	Menu1_1=new Array("What is 'The Code Project'?","/info/guide.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
	Menu1_2=new Array("General FAQ","/info/faq.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
	Menu1_3=new Array("Post a Question","/script/comments/forums.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
	Menu1_4=new Array("Site Directory","/script/articles/sections.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
	Menu1_5=new Array("About Us","/info/about.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
Menu2=new Array("Articles","/info/latest.asp","",6,18,60,"","","","","","",-1,-1,-1,"","");
	Menu2_1=new Array("Latest","/info/latest.asp","",0,18,150,"","","","","","",-1,-1,-1,"","");	
	Menu2_2=new Array("Most Popular","/script/articles/top_articles.asp?st=2","",0,18,150,"","","","","","",-1,-1,-1,"","");	
	Menu2_3=new Array("Search","/info/search.asp","",0,18,150,"","","","","","",-1,-1,-1,"","");		
	Menu2_4=new Array("Site Directory","/script/articles/sections.asp","",0,18,190,"","","","","","",-1,-1,-1,"","");
	Menu2_5=new Array("Submit an Article","/info/submit.asp","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu2_6=new Array("Article Competition","/script/competitions/monthly/","",0,18,150,"","","","","","",-1,-1,-1,"","");
Menu3=new Array("Message Boards","/script/comments/forums.asp","",18,18,120,"","","","","","",-1,-1,-1,"","");
	Menu3_1=new Array("Visual C++","/script/comments/forums.asp?forumid=1647","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_2=new Array("ATL / WTL / STL","/script/comments/forums.asp?forumid=4486","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_3=new Array("COM","/script/comments/forums.asp?forumid=1648","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_4=new Array("Managed C++","/script/comments/forums.asp?forumid=3785","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_5=new Array("C#","/script/comments/forums.asp?forumid=1649","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_6=new Array("ASP.NET","/script/comments/forums.asp?forumid=12076","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_7=new Array("VB.NET","/script/comments/forums.asp?forumid=1646","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_8=new Array("Web Development","/script/comments/forums.asp?forumid=1640","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_9=new Array(".NET Framework","/script/comments/forums.asp?forumid=1650","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_10=new Array("SQL / ADO / ADO.NET","/script/comments/forums.asp?forumid=1725","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_11=new Array("XML / XSL","/script/comments/forums.asp?forumid=3421","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_12=new Array("OS / SysAdmin","/script/comments/forums.asp?forumid=1644","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_13=new Array("Work Issues","/script/comments/forums.asp?forumid=3304","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_14=new Array("Article Requests","/script/comments/forums.asp?forumid=1641","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_15=new Array("Collaboration","/script/comments/forums.asp?forumid=1651","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_16=new Array("General Discussions","/script/comments/forums.asp?forumid=1642","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_17=new Array("Suggestions","/script/comments/forums.asp?forumid=1645","",0,18,150,"","","","","","",-1,-1,-1,"","");
	Menu3_18=new Array("The Soapbox","/script/comments/forums.asp?forumid=2605","",0,18,150,"","","","","","",-1,-1,-1,"","");
Menu4=new Array("StoreFront","/store/","",0,18,80,"","","","","","",-1,-1,-1,"","");
Menu5=new Array("Lounge","/lounge.asp","",0,18,60,"","","","","","",-1,-1,-1,"","");