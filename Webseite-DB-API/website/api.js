let host = "http://localhost"
let port = ":8000"

// document.getElementById('testInfoButtonPost').addEventListener('click', postData);
// document.getElementById('testInfoButtonPut').addEventListener('click', putData)


async function postData(event)
{
  console.log('posting data');

  var request = new XMLHttpRequest();
  let url = host + port + "/test/post";
  request.open('POST', url, true);
  request.setRequestHeader("Content-Type", "application/json");

  request.onreadystatechange = function () {
    if (request.readyState === 4 && request.status === 200) {
        console.log(request.responseText);
    }
  };

  var data = JSON.stringify({ 'test': 'this is a test message' });
  request.send(data);
}


async function putData(event){
  var request = new XMLHttpRequest();
  let url = host + port + "/test/put"

  request.open('PUT', url, true);
  request.setRequestHeader("Content-Type", "application/json");

  var data = JSON.stringify({ 'test2': 'this is the put test messgae' });
  request.send(data);
}


var intervalId = setInterval(function() {
    // getData();
  }, 1000);

function setData(jsonObj){
  var text = document.getElementById("testInfoP");
  text.innerHTML = jsonObj.message;
}

async function getData(){
  await fetch(host + port + '/test/get')
    .then(response => response.json())
    .then(json => setData(json))
    .catch(err => console.log('Request failed', err));
}