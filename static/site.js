var wordSpiralApp = angular.module('wordSpiralApp', ['ui.bootstrap']);

wordSpiralApp.controller('wordListCtrl', function ($scope) {
  $scope.words = [
    {'spell': 'string',
     'id': 1,
     'meanings':[{'type':'n.',
                  'explain':'ひも、糸',
                  'examples':[{'exSrc':'He pulled too hard and the string broke.',
                               'exTgt':'彼があまり引っ張り過ぎて、ひもが切れた。'}],
                 },
                 {'type':'v.',
                  'explain':'弦(ガット)を張る',
                  'examples':[{'exSrc':'I\'ll have my tennis racket strung.',
                               'exTgt':'テニスのラケットにガットを張ってもらおう。'}],
                 }]},
    {'spell': 'fast',
     'id': 2,
     'meanings':[{'type':'adj.',
                  'explain':'速い',
                  'examples':[{'exSrc':'His son learns fast and well.',
                               'exTgt':'彼の息子は勉強が速いし、よく出来ている。'},
                              {'exSrc':'My watch is three minutes fast.',
                               'exTgt':'私の時計は 3 分進んでいる。'}],
                 },
                 {'type':'adj.',
                  'explain':'しっかりした',
                  'examples':[{'exSrc':'The door is fast.',
                               'exTgt':'ドアが締まっている。'}],
                 }]},
  ];

  $scope.currentPlay = 0;
  $scope.debuginfo = "";

  var audio = document.getElementById('audio');
  // set end event
  // will be fired when the audio is finished.
  // NOTE: The event will not be fired when the audio is stoped when it is playing.
  audio.onended = function(){
    $scope.currentPlay = 0;
    $scope.$apply();
  };

  audio.onerror = function(){
    $scope.currentPlay = 0;
    $scope.$apply();
  };

  $scope.audioClick = function(id){
    // stop the playing audio, if any.
    try{
      audio.pause();
      audio.currentTime = 0;
    } catch(e){ }

    // update the current playing id
    $scope.currentPlay = id;

    // load the new audio and play it
    audio.src='/static/audio/' + id + '.mp3';
    audio.load();
    audio.play();
  };

});
