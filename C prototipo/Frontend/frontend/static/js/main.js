document.addEventListener('DOMContentLoaded', () => {
    const content = document.getElementById('content');
  
    // Función que hace fetch de la página y extrae el <main>
    async function loadPage(url, push = true) {
      try {
        const res = await fetch(url);
        const html = await res.text();
        const parser = new DOMParser();
        const doc = parser.parseFromString(html, 'text/html');
        const newMain = doc.getElementById('content');
        if (newMain) {
          content.innerHTML = newMain.innerHTML;
          if (push) history.pushState(null, '', url);
        }
      } catch (err) {
        console.error('Error cargando página:', err);
      }
    }
  
    // Capturar clics en <nav>
    document.querySelectorAll('header nav a').forEach(link => {
      link.addEventListener('click', e => {
        e.preventDefault();
        loadPage(link.href);
      });
    });
  
    // Manejar “atrás” y “adelante” del navegador
    window.addEventListener('popstate', () => {
      loadPage(location.pathname, false);
    });
  });
  