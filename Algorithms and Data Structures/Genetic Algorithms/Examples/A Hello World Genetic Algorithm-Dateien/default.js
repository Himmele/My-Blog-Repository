
function popup(Url, w, h)
{
  settings = "toolbar=no,location=no,directories=no,"+
             "status=no,menubar=no,scrollbars=yes,"+
             "resizable=yes,left=100,top=100,width="+w+",height="+h;

  Popup = window.open(Url, "Generation5Popup", settings);

  Popup.focus() // Bring it to the front.

  if (Popup.opener == null) 
    Popup.opener = self;

}

function popupGlossary(Phrase)
{
  popup("/glossary/popup.asp?uri="+Phrase+".xml", 400, 200);
}

function popupBibTex(Url)
{
  popup("/features/bibtex.asp?url="+Url, 480, 240);
}
