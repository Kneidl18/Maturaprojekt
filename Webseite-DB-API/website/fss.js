var menuepunkte = document.querySelectorAll('.menuleiste li a');



menuepunkte.forEach(function(element) {
  element.addEventListener('click', function() {
    menuepunkte.forEach(function(el) {
      el.classList.remove('aktiv');
    });
    this.classList.add('aktiv');
  });
});

