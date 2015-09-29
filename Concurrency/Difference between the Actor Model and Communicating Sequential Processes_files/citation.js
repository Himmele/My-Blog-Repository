var citation = function () {
    var showCitation = function (jDiv) {
        var id = jDiv.attr("id").substring("cite-post-".length);
        var postMenu = jDiv.parent().parent();
        var popupId = 'cite-popup-' + id;
        var bibradioId = 'bibradio-' + id;
        var amsradioId = 'amsradio-' + id;
        var reftypeId = 'reftype-' + id;
        var exampleId = 'cite-example-' + id;
        var citationTextId = 'cite-textarea-' + id;

        $('#' + popupId).remove();

        var html = '<div id="' + popupId + '" class="popup" style="width:700px;cursor:auto;display:block;">' +
                        '<div id="' + exampleId + '"></div>' +
                        '<input type="radio" id="' + bibradioId + '" name="' + reftypeId + '" checked="checked"> BibTeX</input> ' +
                        '<input type="radio" id="' + amsradioId + '" name="' + reftypeId + '"> amsrefs</input> ' +
                        '<textarea id="' + citationTextId + '" rows="9" style="width:100%;">' +
                        '</textarea><br><p align="right"><a class="cite-close">close</a></p></div>';

        var jcitationDiv = $(html);
        jcitationDiv.find("a.cite-close").click(function () { jcitationDiv.fadeOutAndRemove(); });
        postMenu.append(jcitationDiv);
        StackExchange.helpers.addSpinner(jcitationDiv);

        $.ajax({
            type: "GET",
            url: "/posts/" + id + "/citation",
            dataType: "json",
            success: function (data) {
                StackExchange.helpers.removeSpinner();
                $("#" + exampleId).html(data.example);
                $("#" + citationTextId).val(data.bibtex);
                $("#" + bibradioId).data("ref", data.bibtex).click(function () { $("#" + citationTextId).val($("#" + bibradioId).data("ref")); });
                $("#" + amsradioId).data("ref", data.amsref).click(function () { $("#" + citationTextId).val($("#" + amsradioId).data("ref")); });
            }
        });
    };

    return {
        show: function (jDiv) {
            showCitation(jDiv);
        }
    };
} ();