// 'more' 버튼을 클릭하면 메뉴 섹션이 나타남
document.addEventListener('DOMContentLoaded', function () {
  const moreBtn = document.getElementById('moreBtn');
  const menuSection = document.getElementById('menu-section');

  moreBtn.addEventListener('click', function () {
    // hidden 클래스가 있으면 제거해서 보여주기
    menuSection.classList.toggle('hidden');
    moreBtn.textContent = menuSection.classList.contains('hidden') ? 'MORE' : 'CLOSE';
  });
});
