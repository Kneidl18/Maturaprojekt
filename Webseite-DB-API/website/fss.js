var menuepunkte = document.querySelectorAll('.menuleiste li a');

document.getElementById('testInfoButton').addEventListener('click', postData);


menuepunkte.forEach(function(element) {
  element.addEventListener('click', function() {
    menuepunkte.forEach(function(el) {
      el.classList.remove('aktiv');
    });
    this.classList.add('aktiv');
  });
});


async function postData()
{
  console.log('posting data');

  var request = XMLHttpRequest();
  let url = "http://localhost:8000/test/post";
  request.open('POST', url, true);
  request.setRequestHeader("Content-Type", "applicatoin/json");

  request.onreadystatechange = function () {
    if (request.readyState === 4 && request.status === 200) {
        console.log(request.responseText);
    }

  var data = JSON.stringify({ 'test': 'this is a test message' });
  request.send(data);
};
}


var intervalId = setInterval(function() {
    getData();
  }, 1000);

function setData(jsonObj){
  var text = document.getElementById("testInfoP");
  text.innerHTML = jsonObj.message;
}

async function getData(){
  await fetch('http://localhost:8000/test/get')
    .then(response => response.json())
    .then(json => setData(json))
    .catch(err => console.log('Request failed', err));
}