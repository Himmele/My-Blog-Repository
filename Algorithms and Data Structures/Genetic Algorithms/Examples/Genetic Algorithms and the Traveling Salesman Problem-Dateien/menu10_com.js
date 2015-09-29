/***********************************************
*	(c) Ger Versluis 2000 version 10.71 8 May 2004    *
*	You may use this script on non commercial sites.	          *
*	www.burmees.nl/menu			          *
************************************************/

if(document.getElementById){
	var 	AgntUsr=navigator.userAgent.toLowerCase(),
		AppVer=navigator.appVersion.toLowerCase(),
		Opr7=AgntUsr.indexOf("opera 7")!=-1||AgntUsr.indexOf("opera/7")!=-1?true:false;
	if((AgntUsr.indexOf("opera")==-1||Opr7)){
		var	NavYes=((AgntUsr.indexOf("mozilla")!=-1&&AgntUsr.indexOf("compatible")==-1)||Opr7)?true:false,
			ExpYes=(AgntUsr.indexOf("msie")!=-1&&!Opr7)?	true:false;
		if(MenuUsesFrames&&!parent.frames[0])MenuUsesFrames=0;
		var 	Exp6Plus=((AppVer.indexOf("msie 6")!=-1||AppVer.indexOf("msie 7")!=-1)&&!Opr7)?true:false,
			MacExp5=(AppVer.indexOf("mac")!=-1&&AppVer.indexOf("msie 5")!=-1)?true:false,
			FLoc,ScLoc,DcLoc,SLdAgnWin,FWinH,FColW,SColW,DColW,
			SWinW,SWinH,FWinW,
			RLvl=0,FntFc=1,StaticPos=0,Ztop=100,
			FrstCreat=true,Ldd=false,Crtd=false,FCmplnt=false,SCmplnt=false,M_Clckd=false,IniFlg=false,ShwFlg=false,AcrssFrms=true,
			FrstCntnr=null,CurOvr=null,FHtml=null,ScHtml=null,CloseTmr=null,OpnTmr=null,RmbrNow=null,
			FStr="",M_Hide="hidden",M_Show="visible",P_X="px",
			M_StrtTp=StartTop,M_StrtLft=StartLeft,
			Par=MenuUsesFrames?parent:window,Doc=Par.document,Bod=Doc.body,Trigger=NavYes?Par:Bod,
			Nav_Op=NavYes&&AgntUsr.indexOf("gecko/")!=-1&&parseInt(AgntUsr.substring(AgntUsr.indexOf("gecko/")+6,AgntUsr.indexOf("gecko/")+12))>=200303?true:false,
			InitLdd,LdTmr=setInterval("ChckInitLd()",100)}}

function ChckInitLd(){
	InitLdd=ExpYes?
		MenuUsesFrames?
			Par.frames[FirstLineFrame].document.readyState=="complete"&&Par.frames[SecLineFrame].document.readyState=="complete"?1:0:
			Par.document.readyState=="complete"?1:0:
		MenuUsesFrames?
			Par.frames[FirstLineFrame].document.body&&Par.frames[SecLineFrame].document.body?1:0:
			Par.document.body?1:0;
	if(InitLdd)	{
		clearInterval(LdTmr);
		Go()}}

function CnclSlct(){
	return false}

function RePos(){
	FWinW=ExpYes?FCmplnt?FHtml.clientWidth:FLoc.document.body.clientWidth:FLoc.innerWidth;
	FWinH=ExpYes?FCmplnt?FHtml.clientHeight:FLoc.document.body.clientHeight:FLoc.innerHeight;
	SWinW=ExpYes?SCmplnt?ScHtml.clientWidth:ScLoc.document.body.clientWidth:ScLoc.innerWidth;
	SWinH=ExpYes?SCmplnt?ScHtml.clientHeight:ScLoc.document.body.clientHeight:ScLoc.innerHeight;
	if(MenuCentered.indexOf("justify")!=-1&&FirstLineHorizontal){
		ClcJus();
		var P=FrstCntnr.FrstMbr,W=Menu1[5],	a=BorderBtwnMain?NoOffFirstLineMenus+1:2,i,
			LftXtra=(NavYes&&!Opr7)||MacExp5||FCmplnt?LeftPaddng:0;
		FrstCntnr.style.width=NoOffFirstLineMenus*W*FntFc+a*BorderWidthMain+P_X;
		for(i=0;i<NoOffFirstLineMenus;i++){
			P.style.width=W*FntFc-(P.value.indexOf("<")==-1?LftXtra:0)+P_X;
			if(P.ai&&!RightToLeft)P.ai.style.left=BottomUp?W-Arrws[10]-2+P_X:W-Arrws[4]-2+P_X;
			P=P.PrvMbr}}
	StaticPos=-1;
	ClcRl();
	if(TargetLoc)ClcTrgt();
	ClcLft();
	ClcTp();
	PosMenu(FrstCntnr,StartTop,StartLeft);
	if(RememberStatus)StMnu()}

function UnLdd(){
	Ldd=Crtd=false;
	SetMenu="0";
	var M=FrstCntnr?FrstCntnr.FrstMbr:null;
	if(ExpYes){
		while(M!=null){
			if(M.CCn)	{
				MakeNull(M.CCn);
				M.CCn=null}
			M=M.PrvMbr}}
	LdTmr=setInterval("ChckLdd()",100)}

function UnLddTotal(){
		MakeNull(FrstCntnr);
		FrstCntnr=RmbrNow=FLoc=ScLoc=DcLoc=SLdAgnWin=CurOvr=CloseTmr=Doc=Bod=Trigger=null}

function MakeNull(P){
	var M=P.FrstMbr,Mi;
	while(M!=null){
		Mi=M;
		if(M.CCn)	{
			MakeNull(M.CCn);
			M.CCn=null}
		M=M.PrvMbr;
		Mi.PrvMbr=null;
		Mi=null}
	P.FrstMbr=null}

function ChckLdd(){
	if(!ExpYes){
		if(ScLoc.document.body){
			clearInterval(LdTmr);
			Go()}}
	else if(ScLoc.document.readyState=="complete"){
		if(LdTmr)	clearInterval(LdTmr);
		Go()}}

function Go(){
	if(!Ldd){	
		Crtd=false;
		Ldd=true;
		BeforeStart();
		if(ScaleMenu){
			if(document.defaultView&&document.defaultView.getComputedStyle)
				FntFc=parseInt(document.defaultView.getComputedStyle(document.body,'').getPropertyValue("font-size"))/16;
			else if(ExpYes&&document.body.currentStyle)
				FntFc=parseInt(document.body.currentStyle.fontSize)/12}
		status="Building menu";
		FLoc=MenuUsesFrames?parent.frames[FirstLineFrame]:window;
		ScLoc=MenuUsesFrames?parent.frames[SecLineFrame]:window;
		DcLoc=MenuUsesFrames?parent.frames[DocTargetFrame]:window;
		if(MenuUsesFrames){
			if(!FLoc)FLoc=window;
			if(!ScLoc)ScLoc=window;
			if(!DcLoc)DcLoc=window}
		if(FLoc==ScLoc)AcrssFrms=false;
		if(AcrssFrms)FirstLineHorizontal=MenuFramesVertical?0:FirstLineHorizontal?FirstLineHorizontal:1;
		if(Exp6Plus||Opr7){
			FHtml=FLoc.document.getElementsByTagName("HTML")[0];
			ScHtml=ScLoc.document.getElementsByTagName("HTML")[0];
			FCmplnt=FLoc.document.compatMode.indexOf("CSS")==-1?false:true;
			SCmplnt=ScLoc.document.compatMode.indexOf("CSS")==-1?false:true}
		FWinW=ExpYes?FCmplnt?FHtml.clientWidth:FLoc.document.body.clientWidth:FLoc.innerWidth;
		FWinH=ExpYes?FCmplnt?FHtml.clientHeight:FLoc.document.body.clientHeight:FLoc.innerHeight;
		SWinW=ExpYes?SCmplnt?ScHtml.clientWidth:ScLoc.document.body.clientWidth:ScLoc.innerWidth;
		SWinH=ExpYes?SCmplnt?ScHtml.clientHeight:ScLoc.document.body.clientHeight:ScLoc.innerHeight;
		FColW=FLoc.document.body;
		SColW=ScLoc.document.body;
		DColW=ScLoc.document.body;
		if(TakeOverBgColor)FColW.style.backgroundColor=AcrssFrms?SColW.bgColor:DColW.bgColor;
		if(MenuCentered.indexOf("justify")!=-1&&FirstLineHorizontal)ClcJus();
		if(FrstCreat||FLoc==ScLoc)FrstCntnr=CreateMenuStructure("Menu",NoOffFirstLineMenus,null);
		else CreateMenuStructureAgain("Menu",NoOffFirstLineMenus);
		ClcRl();
		if(TargetLoc)ClcTrgt();
		ClcLft();
		ClcTp();
		PosMenu(FrstCntnr,StartTop,StartLeft);
		Crtd=true;
		SLdAgnWin=ExpYes?ScLoc.document.body:ScLoc;
		SLdAgnWin.onunload=UnLdd;
		if(ExpYes)Trigger.onunload=UnLddTotal;
		Trigger.onresize=RePos;
		AfterBuild();
		if(RememberStatus)StMnu();
		FrstCreat=false;
		if(MenuVerticalCentered=="static"&&!AcrssFrms)setInterval("KeepPos()",250);
		if(!ExpYes)RePos();
		IniFlg=true;
		Initiate();
		status="Menu ready for use"}}

function KeepPos(){
	var TS=ExpYes?SCmplnt?ScHtml.scrollTop:ScLoc.document.body.scrollTop:ScLoc.pageYOffset;
	if(TS!=StaticPos){
		FrstCntnr.OrgTop=StartTop+TS;
		FrstCntnr.style.top=FrstCntnr.OrgTop+P_X;
		StaticPos=TS}}

function ClcRl(){
	StartTop=M_StrtTp<1&&M_StrtTp>0?M_StrtTp*FWinH:M_StrtTp;
	StartLeft=M_StrtLft<1&&M_StrtLft>0?M_StrtLft*FWinW:M_StrtLft}

function ClcJus(){
	var a=BorderBtwnMain?NoOffFirstLineMenus+1:2,
		Sz=Math.round((PartOfWindow*FWinW-a*BorderWidthMain)/NoOffFirstLineMenus),i,j;
	for(i=1;i<NoOffFirstLineMenus+1;i++){
		j=eval("Menu"+i);
		j[5]=Sz}
	StartLeft=0}

function ClcTrgt(){
	var TLoc=FLoc.document.getElementById(TargetLoc);
	while(TLoc){
		StartTop+=TLoc.offsetTop;
		StartLeft+=TLoc.offsetLeft;
		TLoc=TLoc.offsetParent}}

function ClcLft(){
	if(MenuCentered.indexOf("left")==-1){
		var Sz=FWinW-parseInt(FrstCntnr.style.width);
		StartLeft+=MenuCentered.indexOf("right")!=-1?Sz:Sz/2;
		if(StartLeft<0)StartLeft=0}}

function ClcTp(){
	if(MenuVerticalCentered!="top"&&MenuVerticalCentered!="static"){
		var Sz=FWinH-parseInt(FrstCntnr.style.height);
		StartTop+=MenuVerticalCentered=="bottom"?Sz:Sz/2;
		if(StartTop<0)StartTop=0}}

function PosMenu(Ct,Tp,Lt){
	RLvl++;
	var Cmplnt=RLvl==1?FCmplnt:SCmplnt,
		LftXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?LeftPaddng:0,
		TpXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?TopPaddng:0,
		Mb=Ct.FrstMbr,CStl=Ct.style,MStl=Mb.style,
		PadL=Mb.value.indexOf("<")==-1?LftXtra:0,
		PadT=Mb.value.indexOf("<")==-1?TpXtra:0,
		MWt=parseInt(MStl.width)+PadL,
		MHt=parseInt(MStl.height)+PadT,
		CWt=parseInt(CStl.width),
		CHt=parseInt(CStl.height),
		CCw,CCh,STp,SLt,Ti,Li,Hi,
		BRW=RLvl==1?BorderWidthMain:BorderWidthSub,
		BTWn=RLvl==1?BorderBtwnMain:BorderBtwnSub;
	if(RLvl==1&&AcrssFrms)
		!MenuFramesVertical?Tp=BottomUp?DistFrmFrameBrdr:FWinH-CHt-DistFrmFrameBrdr:
			Lt=RightToLeft?DistFrmFrameBrdr:FWinW-CWt-DistFrmFrameBrdr;
	if(RLvl==2&&AcrssFrms)
		!MenuFramesVertical?Tp=BottomUp?SWinH-CHt:0:Lt=RightToLeft?SWinW-CWt:0;
	if(RLvl==2){
		Tp+=VerCorrect;
		Lt+=HorCorrect}
	CStl.top=RLvl==1?Tp+P_X:0;
	Ct.OrgTop=Tp;
	CStl.left=RLvl==1?Lt+P_X:0;
	Ct.OrgLeft=Lt;
	if(RLvl<=FirstLineHorizontal){
		Hi=1;
		Li=CWt-MWt-2*BRW;
		Ti=0}
	else{	Hi=Li=0;
		Ti=CHt-MHt-2*BRW}
	while(Mb!=null){
		MStl.left=Li+BRW+P_X;
		MStl.top=Ti+BRW+P_X;
		if(Mb.CCn){
			if(RightToLeft)CCw=parseInt(Mb.CCn.style.width);
			if(BottomUp)CCh=parseInt(Mb.CCn.style.height);
			if(Hi){	STp=BottomUp?
				Ti-CCh:Ti+MHt+2*BRW;
				SLt=RightToLeft?Li+MWt-CCw:Li}
			else{	SLt=RightToLeft?Li-CCw+ChildOverlap*MWt+BRW:Li+(1-ChildOverlap)*MWt+BRW;
				STp=RLvl==1&&AcrssFrms?
					BottomUp?Ti-CCh+MHt:Ti:
					BottomUp?Ti-CCh+(1-ChildVerticalOverlap)*MHt+2*BRW:Ti+ChildVerticalOverlap*MHt}
			PosMenu(Mb.CCn,STp,SLt)}
		Mb=Mb.PrvMbr;
		if(Mb){	MStl=Mb.style;
			PadL=Mb.value.indexOf("<")==-1?LftXtra:0;
			PadT=Mb.value.indexOf("<")==-1?TpXtra:0;
			MWt=parseInt(MStl.width)+PadL;
			MHt=parseInt(MStl.height)+PadT;
			Hi?Li-=BTWn?(MWt+BRW):(MWt):Ti-=BTWn?(MHt+BRW):MHt}}
	RLvl--}

function StMnu(){
	if(Crtd){
		var i,Pntr=FrstCntnr,Str=ScLoc.SetMenu?ScLoc.SetMenu:"0";
		while(Str.indexOf("_")!=-1&&RememberStatus==1){
			i=Pntr.NrItms-parseInt(Str.substring(0,Str.indexOf("_")));
			Str=Str.slice(Str.indexOf("_")+1);
			Pntr=Pntr.FrstMbr;
			for(i;i;i--)	Pntr=Pntr.PrvMbr;
			Pntr.OM();
			Pntr=Pntr.CCn}
		i=Pntr.NrItms-parseInt(Str);
		Pntr=Pntr.FrstMbr;
		for(i;i;i--)	Pntr=Pntr.PrvMbr;
		if(RmbrNow!=null){
			SetItem(RmbrNow,0);
			RmbrNow.Clckd=0}
		if(Pntr!=null){
			SetItem(Pntr,1);
			Pntr.Clckd=1;
			if(RememberStatus==1)Pntr.OM()}
		RmbrNow=Pntr;
		ClrAllChlds(FrstCntnr.FrstMbr);
		Rmbr(FrstCntnr)}}

function Initiate(){
	if(IniFlg&&Ldd){
		Init(FrstCntnr);
		IniFlg=false;
		if(RememberStatus)Rmbr(FrstCntnr);
		if(ShwFlg)AfterCloseAll();
		ShwFlg=false;
		M_Clckd=false}}

function Rmbr(CntPtr){
	var Mbr=CntPtr.FrstMbr,St;
	while(Mbr!=null){
		if(Mbr.DoRmbr){
			HiliteItem(Mbr);
			if(Mbr.CCn&&RememberStatus==1){
				St=Mbr.CCn.style;
				St.visibility=M_Show;
				Rmbr(Mbr.CCn)}
			break}
		else Mbr=Mbr.PrvMbr}}

function Init(CPt){
	var Mb=CPt.FrstMbr,MCSt=CPt.style;
	RLvl++;
	MCSt.visibility=RLvl==1?M_Show:M_Hide;
	CPt.Shw=RLvl==1?1:0;
	while(Mb!=null){
		if(Mb.Hilite)LowItem(Mb);
		if(Mb.CCn)Init(Mb.CCn);
		Mb=Mb.PrvMbr}
	RLvl--}

function ClrAllChlds(Pt){
	var PSt,Pc;
	while(Pt){
		if(Pt.Hilite){
			Pc=Pt;
			if(Pc!=CurOvr)LowItem(Pt);
			if(Pt.CCn)	{
				PSt=Pt.CCn.style;
				if(Pc!=CurOvr){
					PSt.visibility=M_Hide;
					Pt.CCn.Shw=0}
				ClrAllChlds(Pt.CCn.FrstMbr)}
			break}
		Pt=Pt.PrvMbr}}

function SetItem(Pntr,x){
	while(Pntr!=null){
		Pntr.DoRmbr=x;
		Pntr=Pntr.Ctnr.Cllr}}

function GoTo(){
	if(UnfoldsOnClick&&!M_Clckd&&this.Arr[3]){
		M_Clckd=true;
		OpenMenuII(this)}
	else if(this.Arr[1]){
		status="";
		LowItem(this);
		IniFlg=true;
		Initiate();
		if(this.Arr[1].indexOf("javascript:")!=-1)
			eval(this.Arr[1]);
		else if(DcLoc.location.href!=BaseHref+this.Arr[1]){
			DcLoc.location.href=BaseHref+this.Arr[1]}}}

function HiliteItem(P){
	var PS=P.style;
	if(P.rob)PS.backgroundImage="url(\""+P.rib2+"\")";
	if(P.ro){	var Lc=P.Lvl==1?FLoc:ScLoc;
		Lc.document.images[P.rid].src=P.ri2}
	else{	if(P.Arr[7])PS.backgroundColor=P.Arr[7];
		if(P.Arr[9])PS.color=P.Arr[9];
		if(HooverBold)PS.fontWeight="bold";
		if(HooverItalic)PS.fontStyle="italic";
		if(HooverUnderLine)PS.textDecoration="underline";
		if(HooverTextSize)PS.fontSize=parseInt(PS.fontSize)+HooverTextSize+P_X;
		if(HooverVariant)PS.fontVariant="small-caps"}
		if(HoverBorder){PS.borderColor=HoverBorderColor;PS.borderWidth=BorderWidthHover;PS.borderStyle='solid';}	
	P.Hilite=1}

function LowItem(P){
	var PS=P.style;
	P.Hilite=0;
	if(P.rob)PS.backgroundImage="url(\""+P.rib1+"\")";
	if(P.ro){	var Lc=P.Lvl==1?FLoc:ScLoc;
		Lc.document.images[P.rid].src=P.ri1}
	else{	if(P.Arr[6])PS.backgroundColor=P.Arr[6];
		if(P.Arr[8])PS.color=P.Arr[8];
		if(HooverBold)PS.fontWeight="normal";
		if(HooverItalic)PS.fontStyle="normal";
		if(HooverUnderLine)PS.textDecoration="none";
		if(HooverTextSize)PS.fontSize=parseInt(PS.fontSize)-HooverTextSize+P_X;
		if(HooverVariant)PS.fontVariant="normal"}
		if(HoverBorder){PS.borderColor=PS.backgroundColor;PS.borderWidth=0;PS.borderStyle='none';}	
		}

function OpenMenu(){
	if(Ldd&&Crtd){
		if(UnfoldsOnClick&&!M_Clckd){	
			CurOvr=this;
			IniFlg=false;
			ClrAllChlds(this.Ctnr.FrstMbr);
			HiliteItem(this);
			status=this.Arr[16]}
		else 	OpenMenuII(this)}}

function OpenMenuII(Ptr){
	if(OpnTmr)clearTimeout(OpnTmr);
	if(Ptr.NofChlds&&!Ptr.CCn){
		var Cmplnt=RLvl==1?FCmplnt:SCmplnt,
			LftXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?LeftPaddng:0,
			TpXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?TopPaddng:0;
		RLvl=Ptr.Lvl;
		Ptr.CCn=CreateMenuStructure(Ptr.MN+"_",Ptr.NofChlds,Ptr);
		var Ti,Li,Hi,MStl=Ptr.style,CCw,CCh,STp,SLt,
			PadL=Ptr.value.indexOf("<")==-1?LftXtra:0,
			PadT=Ptr.value.indexOf("<")==-1?TpXtra:0,
			MWt=parseInt(MStl.width)+PadL,
			MHt=parseInt(MStl.height)+PadT,
			BRW=RLvl==1?BorderWidthMain:BorderWidthSub;
		if(RightToLeft)CCw=parseInt(Ptr.CCn.style.width);
		if(BottomUp)CCh=parseInt(Ptr.CCn.style.height);
		if(RLvl<=FirstLineHorizontal){
			Hi=1;
			Li=parseInt(Ptr.style.left)-BRW;
			Ti=0}
		else{	Hi=Li=0;
			Ti=parseInt(Ptr.style.top)-BRW}
		if(Hi){	STp=BottomUp?Ti-CCh:Ti+MHt+2*BRW;
			SLt=RightToLeft?Li+MWt-CCw:Li}
		else{	SLt=RightToLeft?Li-CCw+ChildOverlap*MWt+BRW:Li+(1-ChildOverlap)*MWt;
			STp=RLvl==1&&AcrssFrms?
				BottomUp?Ti-CCh+MHt:Ti:
				BottomUp?Ti-CCh+(1-ChildVerticalOverlap)*MHt+2*BRW:Ti+ChildVerticalOverlap*MHt+BRW}
		PosMenu(Ptr.CCn,STp,SLt);
		RLvl=0}
	var CCnt=Ptr.CCn,HP=Ptr;
	CurOvr=Ptr;
	IniFlg=false;
	ClrAllChlds(Ptr.Ctnr.FrstMbr);
	if(!HP.Hilite)HiliteItem(HP);
	if(CCnt!=null&&!CCnt.Shw)RememberStatus?Unfld():OpnTmr=setTimeout("Unfld()",UnfoldDelay);
	status=HP.Arr[16]}

function Unfld(){
	var P=CurOvr,PSt=P.style,CSt=P.Ctnr.style,CCnt=P.CCn,CCSt=CCnt.style,
		THt=parseInt(PSt.height),TWt=parseInt(PSt.width),
		TLt=AcrssFrms&&P.Lvl==1&&!FirstLineHorizontal?0:parseInt(CSt.left),
		TTp=AcrssFrms&&P.Lvl==1&&FirstLineHorizontal?0:parseInt(CSt.top),
		CCW=parseInt(CCSt.width),
		CCH=parseInt(CCSt.height),
		TS=ExpYes?SCmplnt?ScHtml.scrollTop:ScLoc.document.body.scrollTop:ScLoc.pageYOffset,
		LS=ExpYes?SCmplnt?ScHtml.scrollLeft:ScLoc.document.body.scrollLeft:ScLoc.pageXOffset,
		SLt=AcrssFrms&&P.Lvl==1?CCnt.OrgLeft+TLt+LS:CCnt.OrgLeft+TLt,
		STp=AcrssFrms&&P.Lvl==1?	CCnt.OrgTop+TTp+TS:CCnt.OrgTop+TTp;
	if(!ShwFlg){
		ShwFlg=true;
		BeforeFirstOpen()}
	if(MenuWrap){
		if(RightToLeft){
			if(SLt<LS)SLt=P.Lvl==1?LS:SLt+(CCW+(1-2*ChildOverlap)*TWt);
			if(SLt+CCW>SWinW+LS)SLt=SWinW+LS-CCW}
		else{	if(SLt+CCW>SWinW+LS)SLt=P.Lvl==1?SWinW+LS-CCW:SLt-(CCW+(1-2*ChildOverlap)*TWt);
			if(SLt<LS)SLt=LS}
		if(BottomUp){
			if(STp<TS)STp=P.Lvl==1?TS:STp+(CCH-(1-2*ChildVerticalOverlap)*THt);
			if(STp+CCH>SWinH+TS)STp=SWinH+TS-CCH+0}
		else{	if(STp+CCH>TS+SWinH)STp=P.Lvl==1?STp=TS+SWinH-CCH:STp-CCH+(1-2*ChildVerticalOverlap)*THt;
			if(STp<TS)STp=TS}}
	CCSt.top=STp+P_X;
	CCSt.left=SLt+P_X;
	if(Exp6Plus&&MenuSlide){
		CCnt.filters[0].Apply();
		CCnt.filters[0].play()}
	CCSt.visibility=M_Show}

function CloseMenu(){
	if(Ldd&&Crtd){
		status="";
		if(this==CurOvr){
			if(OpnTmr)clearTimeout(OpnTmr);
			if(CloseTmr)clearTimeout(CloseTmr);
			IniFlg=true;
			CloseTmr=setTimeout("Initiate(CurOvr)",DissapearDelay)}}}

function CntnrSetUp(W,H,NoOff,WMu,Mc){
	var x=eval(WMu+"[10]")!=""?eval(WMu+"[10]"):BorderColor,TS=this.style;
	this.FrstMbr=null;
	this.NrItms=NoOff;
	this.Cllr=Mc;
	this.Shw=0;
	this.OrgLeft=this.OrgTop=0;
	if(x)TS.backgroundColor=x;
	TS.width=W+P_X;
	TS.height=H+P_X;
	TS.zIndex=RLvl+Ztop;
	if(Nav_Op&&!Opr7&&MenuOpacity)
		TS.MozOpacity=parseInt(MenuOpacity.substring(MenuOpacity.indexOf("=")+1,MenuOpacity.indexOf(")")))/125;
	if(Exp6Plus){
		FStr="";
		if(MenuSlide&&RLvl!=1)FStr=MenuSlide;
		if(MenuShadow)FStr+=MenuShadow;
		if(MenuOpacity)FStr+=MenuOpacity;
		if(FStr!="")TS.filter=FStr}}

function MbrSetUp(MbC,PrMmbr,WMu,Wd,Ht,Nofs){
	var Cmplnt=RLvl==1?FCmplnt:SCmplnt,
		LftXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?LeftPaddng:0,
		TpXtra=(NavYes&&!Opr7)||MacExp5||Cmplnt?TopPaddng:0,
		Lctn=RLvl==1?FLoc:ScLoc,
		Tfld=this.Arr[0],t,T,L,a,
		TS=this.style,TA2=this.Arr[2];
	this.PrvMbr=PrMmbr;
	this.Lvl=RLvl;
	this.Ctnr=MbC;
	this.CCn=null;
	this.ai=null;
	this.Hilite=0;
	this.DoRmbr=0;
	this.Clckd=0;
	this.OM=OpenMenu;
	TS.overflow="hidden";
	this.MN=WMu;
	this.NofChlds=Nofs;
	TS.cursor=(this.Arr[1]||(RLvl==1&&UnfoldsOnClick))?ExpYes?"hand":"pointer":"default";
	this.ro=this.rob=0;
	if(Tfld.indexOf("rollover")!=-1){
		this.ro=1;
		this.ri1=Tfld.substring(Tfld.indexOf("?")+1,Tfld.lastIndexOf("?"));
		this.ri2=Tfld.substring(Tfld.lastIndexOf("?")+1,Tfld.length);this.rid=WMu+"i";
		Tfld="<img src=\""+this.ri1+"\" name=\""+this.rid+"\" width=\""+Wd+"\" height=\""+Ht+"\">"}
	this.value=Tfld;
	TS.color=this.Arr[8];
	TS.fontFamily=this.Arr[11];
	TS.fontSize=this.Arr[12]*(NavYes?1:FntFc)+"px";
	TS.fontWeight=this.Arr[13]?"bold":"normal";
	TS.fontStyle=this.Arr[14]?"italic":"normal";
	if(this.Arr[6]&&!this.ro)TS.backgroundColor=this.Arr[6];
	TS.textAlign=this.Arr[15];
	if(TA2){	if(TA2.indexOf("rollover")==-1)TS.backgroundImage="url(\""+TA2+"\")";
		else{	this.rob=1;
			this.rib1=TA2.substring(TA2.indexOf("?")+1,TA2.lastIndexOf("?"));
			this.rib2=TA2.substring(TA2.lastIndexOf("?")+1,TA2.length);
			TS.backgroundImage="url(\""+this.rib1+"\")";
			TS.backgroundRepeat="no-repeat";
			TS.backgroundPosition=BgImgLeftOffset+"px"}}
	if(Tfld.indexOf("<")==-1){
		TS.width=Wd-LftXtra+P_X;
		TS.height=Ht-TpXtra+P_X;
		TS.paddingLeft=LeftPaddng+P_X;
		TS.paddingTop=TopPaddng+P_X}
	else{	TS.width=Wd+P_X;
		TS.height=Ht+P_X}
	if(Tfld.indexOf("<")==-1){
		t=Lctn.document.createTextNode(Tfld);
		this.appendChild(t)}
	else this.innerHTML=Tfld;
	if(this.Arr[3]){
		a=RLvl<=FirstLineHorizontal?BottomUp?9:3:RightToLeft?6:0;
		if(Arrws[a]!=""){
			T=RLvl<=FirstLineHorizontal?BottomUp?2:Ht-Arrws[a+2]-2:(Ht-Arrws[a+2])/2;
			L=RightToLeft?2:Wd-Arrws[a+1]-2;t=Lctn.document.createElement("img");
			this.appendChild(t);
			t.style.position="absolute";
			t.src=Arrws[a];
			t.style.width=Arrws[a+1]*FntFc+P_X;
			t.style.height=Arrws[a+2]*FntFc+P_X;
			t.style.top=T+P_X;
			t.style.left=L+P_X;
			this.ai=t}}
	if(ExpYes){
		this.onselectstart=CnclSlct;
		this.onmouseover=OpenMenu;
		this.onmouseout=CloseMenu;
		this.onclick=GoTo}
	else{	this.addEventListener("mouseover",OpenMenu,false);
		this.addEventListener("mouseout",CloseMenu,false);
		this.addEventListener("click",GoTo,false)}}

function CreateMenuStructure(MNm,No,Mcllr){
	RLvl++;
	var i,NOs,Mbr,W=0,H=0,PMb=null,WMnu=MNm+"1",
		MWd=eval(WMnu+"[5]")*FntFc,
		MHt=eval(WMnu+"[4]")*FntFc,
		Lctn=RLvl==1?FLoc:ScLoc,
		BRW=RLvl==1?BorderWidthMain:BorderWidthSub,
		BTWn=RLvl==1?BorderBtwnMain:BorderBtwnSub;
	if(RLvl<=FirstLineHorizontal){
		for(i=1;i<No+1;i++){
			WMnu=MNm+eval(i);
			W=eval(WMnu+"[5]")?W+eval(WMnu+"[5]")*FntFc:W+MWd}
		W=BTWn?W+(No+1)*BRW:W+2*BRW;
		H=MHt+2*BRW}
	else{	for(i=1;i<No+1;i++){
			WMnu=MNm+eval(i);
			H=eval(WMnu+"[4]")?H+eval(WMnu+"[4]")*FntFc:H+MHt}
		H=BTWn?H+(No+1)*BRW:H+2*BRW;
		W=MWd+2*BRW}
	var MbC=Lctn.document.createElement("div");
	MbC.style.position="absolute";
	MbC.style.visibility="hidden";
	Lctn.document.body.appendChild(MbC);
	MbC.SetUp=CntnrSetUp;
	MbC.SetUp(W,H,No,MNm+"1",Mcllr);
	for(i=1;i<No+1;i++){
		WMnu=MNm+eval(i);
		NOs=eval(WMnu+"[3]");
		W=RLvl<=FirstLineHorizontal?eval(WMnu+"[5]")?eval(WMnu+"[5]")*FntFc:MWd:MWd;
		H=RLvl<=FirstLineHorizontal?MHt:eval(WMnu+"[4]")?eval(WMnu+"[4]")*FntFc:MHt;
		Mbr=Lctn.document.createElement("div");
		Mbr.style.position="absolute";
		Mbr.style.visibility="inherit";
		MbC.appendChild(Mbr);
		Mbr.Arr=eval(WMnu);
		if(Mbr.Arr[6]=="")Mbr.Arr[6]=LowBgColor;
		if(Mbr.Arr[7]=="")Mbr.Arr[7]=HighBgColor;
		if(Mbr.Arr[8]=="")Mbr.Arr[8]=FontLowColor;
		if(Mbr.Arr[9]=="")Mbr.Arr[9]=FontHighColor;
		if(Mbr.Arr[11]=="")Mbr.Arr[11]=FontFamily;
		if(Mbr.Arr[12]==-1)Mbr.Arr[12]=FontSize;
		if(Mbr.Arr[13]==-1)Mbr.Arr[13]=FontBold;
		if(Mbr.Arr[14]==-1)Mbr.Arr[14]=FontItalic;
		if(Mbr.Arr[15]=="")Mbr.Arr[15]=MenuTextCentered;
		if(Mbr.Arr[16]=="")Mbr.Arr[16]=Mbr.Arr[1];
		Mbr.SetUp=MbrSetUp;
		Mbr.SetUp(MbC,PMb,WMnu,W,H,NOs);
		if(NOs&&!BuildOnDemand){
			Mbr.CCn=CreateMenuStructure(WMnu+"_",NOs,Mbr)}
		PMb=Mbr}
	MbC.FrstMbr=Mbr;
	RLvl--;
	return(MbC)}

function CreateMenuStructureAgain(MNm,No){
	var Mbr=FrstCntnr.FrstMbr;
	if(!BuildOnDemand)	{
		var i,WMnu,NOs,PMb;
		RLvl++;
		for(i=No;i>0;i--){
			WMnu=MNm+eval(i);
			NOs=eval(WMnu+"[3]");
			PMb=Mbr;
			if(NOs)Mbr.CCn=CreateMenuStructure(WMnu+"_",NOs,Mbr);
			Mbr=Mbr.PrvMbr}
		RLvl--}
	else{	
		while(Mbr){
			Mbr.CCn=null;
			Mbr=Mbr.PrvMbr}}}