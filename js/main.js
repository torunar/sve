function getCaret(element) {
  var caretOffset = 0;
  var doc = element.ownerDocument || element.document;
  var win = doc.defaultView || doc.parentWindow;
  var sel;
  if (typeof win.getSelection != "undefined") {
      var range = win.getSelection().getRangeAt(0);
      var preCaretRange = range.cloneRange();
      preCaretRange.selectNodeContents(element);
      preCaretRange.setEnd(range.endContainer, range.endOffset);
      caretOffset = preCaretRange.toString().length;
  } else if ( (sel = doc.selection) && sel.type != "Control") {
      var textRange = sel.createRange();
      var preCaretTextRange = doc.body.createTextRange();
      preCaretTextRange.moveToElementText(element);
      preCaretTextRange.setEndPoint("EndToEnd", textRange);
      caretOffset = preCaretTextRange.text.length;
  }
  return caretOffset;
}

function putCaret(el, sPos) {
  var charIndex = 0;
  var range = document.createRange();
  range.setStart(el, 0);
  range.collapse(true);
  var nodeStack = [el];
  var node;
  var foundStart = false;
  var stop = false;

  while (!stop && (node = nodeStack.pop())) {
    if (node.nodeType == 3) {
      var nextCharIndex = charIndex + node.length;
      if (!foundStart && sPos >= charIndex && sPos <= nextCharIndex) {
        range.setStart(node, sPos - charIndex);
        foundStart = true;
      }
      if (foundStart && sPos >= charIndex && sPos <= nextCharIndex) {
        range.setEnd(node, sPos - charIndex);
        stop = true;
      }
      charIndex = nextCharIndex;
    }
    else {
      var i = node.childNodes.length;
      while (i--) {
        nodeStack.push(node.childNodes[i]);
      }
    }
  }
  selection = window.getSelection();
  selection.removeAllRanges();
  selection.addRange(range);
}

function rehighlight(element) {
  var plainText = element.innerHTML.replace(/(<[\/]?span(.*?)>)/ig, '');
  element.innerHTML = plainText;
  hljs.highlightBlock(element);
}

var h = document.getElementById('code');
hljs.configure({useBR: true});
hljs.highlightBlock(h);

var refresh = false;

h.onkeydown = function(event) {
  if (event.keyCode == 9)
    return false;
  if ((event.keyCode == 32 || event.keyCode == 13))
    refresh = true;
};

h.onkeyup = function(event) {
  if (refresh) {
    pos = getCaret(h);
    rehighlight(h);
    /* some fix for enter pos must be here */
    putCaret(h, pos);
    refresh = false;
  }
}