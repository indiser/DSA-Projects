/**
 * app.js — Main Application Entry Point
 * DSA Project Roadmap Documentation
 *
 * Initializes all modules and handles:
 * - Navbar scroll behaviour
 * - Smooth scrolling
 * - Scroll-reveal animations
 * - Counter animations
 * - Particle canvas
 * - Ripple effects
 * - Back-to-top button
 * - Active section tracking
 */

(function App() {
  'use strict';

  // ─────────────────────────────────────
  // NAVBAR SCROLL BEHAVIOUR
  // ─────────────────────────────────────

  function initNavbar() {
    const navbar = document.querySelector('.navbar');
    if (!navbar) return;

    function onScroll() {
      navbar.classList.toggle('scrolled', window.scrollY > 20);
    }

    window.addEventListener('scroll', onScroll, { passive: true });
    onScroll();
  }

  // ─────────────────────────────────────
  // HAMBURGER MENU
  // ─────────────────────────────────────

  function initMobileMenu() {
    const hamburger  = document.getElementById('hamburger');
    const mobileMenu = document.getElementById('mobile-menu');
    if (!hamburger || !mobileMenu) return;

    hamburger.addEventListener('click', () => {
      const isOpen = mobileMenu.classList.contains('open');
      mobileMenu.classList.toggle('open', !isOpen);
      hamburger.classList.toggle('open', !isOpen);
      hamburger.setAttribute('aria-expanded', String(!isOpen));
    });

    // Close on link click
    mobileMenu.querySelectorAll('a').forEach((link) => {
      link.addEventListener('click', () => {
        mobileMenu.classList.remove('open');
        hamburger.classList.remove('open');
        hamburger.setAttribute('aria-expanded', 'false');
      });
    });

    // Close on outside click
    document.addEventListener('click', (e) => {
      if (!hamburger.contains(e.target) && !mobileMenu.contains(e.target)) {
        mobileMenu.classList.remove('open');
        hamburger.classList.remove('open');
        hamburger.setAttribute('aria-expanded', 'false');
      }
    });
  }

  // ─────────────────────────────────────
  // SMOOTH SCROLLING
  // ─────────────────────────────────────

  function initSmoothScroll() {
    document.querySelectorAll('a[href^="#"]').forEach((link) => {
      link.addEventListener('click', (e) => {
        const href = link.getAttribute('href');
        if (href === '#') return;

        const target = document.querySelector(href);
        if (!target) return;

        e.preventDefault();
        const navbarHeight = parseInt(
          getComputedStyle(document.documentElement)
            .getPropertyValue('--navbar-height'),
          10
        ) || 64;

        const top = target.getBoundingClientRect().top + window.scrollY - navbarHeight - 16;
        window.scrollTo({ top, behavior: 'smooth' });
      });
    });
  }

  // ─────────────────────────────────────
  // ACTIVE SECTION TRACKING
  // ─────────────────────────────────────

  function initActiveSection() {
    const sections = document.querySelectorAll('section[id]');
    const navLinks = document.querySelectorAll('.navbar__nav-link, .navbar__mobile-link');

    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          const id = entry.target.id;
          navLinks.forEach((link) => {
            const href = link.getAttribute('href');
            link.classList.toggle('active', href === `#${id}`);
          });
        }
      });
    }, {
      rootMargin: '-40% 0px -55% 0px',
      threshold: 0,
    });

    sections.forEach((s) => observer.observe(s));
  }

  // ─────────────────────────────────────
  // SCROLL-REVEAL ANIMATIONS
  // ─────────────────────────────────────

  function initReveal() {
    const reveals = document.querySelectorAll('.reveal');
    if (!reveals.length) return;

    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          entry.target.classList.add('visible');
          observer.unobserve(entry.target);
        }
      });
    }, { threshold: 0.1, rootMargin: '0px 0px -40px 0px' });

    reveals.forEach((el) => observer.observe(el));
  }

  // ─────────────────────────────────────
  // COUNTER ANIMATIONS
  // ─────────────────────────────────────

  /**
   * Animate a number counter from 0 to target
   * @param {Element} el  - DOM element
   * @param {number} target - target value
   * @param {number} duration - animation duration ms
   * @param {string} suffix - text appended after number
   */
  function animateCounter(el, target, duration, suffix = '') {
    const start     = performance.now();
    const startVal  = 0;

    function step(now) {
      const elapsed  = now - start;
      const progress = Math.min(elapsed / duration, 1);
      // Ease out cubic
      const eased = 1 - Math.pow(1 - progress, 3);
      const current = Math.round(startVal + (target - startVal) * eased);

      el.textContent = current.toLocaleString() + suffix;

      if (progress < 1) {
        requestAnimationFrame(step);
      } else {
        el.textContent = target.toLocaleString() + suffix;
      }
    }

    requestAnimationFrame(step);
  }

  function initCounters() {
    const counters = document.querySelectorAll('[data-count]');
    if (!counters.length) return;

    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          const el     = entry.target;
          const target = parseFloat(el.dataset.count);
          const suffix = el.dataset.suffix || '';
          const dur    = parseInt(el.dataset.duration || '1500', 10);
          animateCounter(el, target, dur, suffix);
          observer.unobserve(el);
        }
      });
    }, { threshold: 0.5 });

    counters.forEach((el) => observer.observe(el));
  }

  // ─────────────────────────────────────
  // PARTICLE CANVAS
  // ─────────────────────────────────────

  function initParticles() {
    const canvas = document.getElementById('particle-canvas');
    if (!canvas) return;

    const ctx = canvas.getContext('2d');
    let width, height, particles;
    let animFrame;
    let isDark = document.documentElement.getAttribute('data-theme') === 'dark';

    function resize() {
      width  = canvas.width  = canvas.offsetWidth;
      height = canvas.height = canvas.offsetHeight;
    }

    function getParticleColor() {
      const r = Math.random();
      if (isDark) {
        return `rgba(${r > 0.5 ? '227,38,28' : '34,197,94'}, `;
      }
      return `rgba(${r > 0.5 ? '227,38,28' : '22,163,74'}, `;
    }

    function createParticle() {
      const color = getParticleColor();
      return {
        x:       Math.random() * width,
        y:       height + Math.random() * 100,
        vx:      (Math.random() - 0.5) * 0.4,
        vy:      -(Math.random() * 0.5 + 0.3),
        radius:  Math.random() * 1.5 + 0.5,
        opacity: Math.random() * 0.4 + 0.1,
        maxLife: Math.random() * 400 + 200,
        life:    0,
        color,
      };
    }

    function initParticleArray() {
      particles = Array.from({ length: 60 }, createParticle);
    }

    function drawParticle(p) {
      const alpha = p.opacity * (1 - p.life / p.maxLife);
      ctx.beginPath();
      ctx.arc(p.x, p.y, p.radius, 0, Math.PI * 2);
      ctx.fillStyle = p.color + alpha + ')';
      ctx.fill();
    }

    function tick() {
      ctx.clearRect(0, 0, width, height);

      particles.forEach((p, i) => {
        p.x   += p.vx;
        p.y   += p.vy;
        p.life++;

        drawParticle(p);

        // Respawn when off-screen or expired
        if (p.y < -20 || p.life > p.maxLife) {
          particles[i] = createParticle();
        }
      });

      animFrame = requestAnimationFrame(tick);
    }

    window.addEventListener('resize', () => {
      resize();
      initParticleArray();
    }, { passive: true });

    document.addEventListener('themechange', (e) => {
      isDark = e.detail.theme === 'dark';
      initParticleArray();
    });

    // Pause when tab hidden
    document.addEventListener('visibilitychange', () => {
      if (document.hidden) {
        cancelAnimationFrame(animFrame);
      } else {
        tick();
      }
    });

    resize();
    initParticleArray();
    tick();
  }

  // ─────────────────────────────────────
  // RIPPLE EFFECT ON BUTTONS
  // ─────────────────────────────────────

  function initRipple() {
    document.querySelectorAll('.btn').forEach((btn) => {
      // Create ripple container
      const container = document.createElement('span');
      container.className = 'ripple-container';
      btn.appendChild(container);

      btn.addEventListener('click', (e) => {
        const rect   = btn.getBoundingClientRect();
        const size   = Math.max(rect.width, rect.height) * 2;
        const x      = e.clientX - rect.left - size / 2;
        const y      = e.clientY - rect.top  - size / 2;

        const ripple = document.createElement('span');
        ripple.className = 'ripple';
        ripple.style.cssText = `
          width:  ${size}px;
          height: ${size}px;
          left:   ${x}px;
          top:    ${y}px;
        `;

        container.appendChild(ripple);
        ripple.addEventListener('animationend', () => ripple.remove());
      });
    });
  }

  // ─────────────────────────────────────
  // BACK TO TOP
  // ─────────────────────────────────────

  function initBackToTop() {
    const btn = document.getElementById('back-to-top');
    if (!btn) return;

    window.addEventListener('scroll', () => {
      btn.classList.toggle('visible', window.scrollY > 400);
    }, { passive: true });

    btn.addEventListener('click', () => {
      window.scrollTo({ top: 0, behavior: 'smooth' });
    });
  }

  // ─────────────────────────────────────
  // TOAST NOTIFICATION
  // ─────────────────────────────────────

  function showToast(message, icon = '✓', duration = 3000) {
    let toast = document.getElementById('toast');
    if (!toast) {
      toast = document.createElement('div');
      toast.id = 'toast';
      toast.className = 'toast';
      toast.setAttribute('role', 'status');
      toast.setAttribute('aria-live', 'polite');
      document.body.appendChild(toast);
    }

    toast.innerHTML = `<span class="toast__icon">${icon}</span> ${message}`;
    toast.classList.add('show');

    clearTimeout(toast.__timeout);
    toast.__timeout = setTimeout(() => {
      toast.classList.remove('show');
    }, duration);
  }

  // ─────────────────────────────────────
  // COPY CODE SNIPPETS
  // ─────────────────────────────────────

  function initCopyCode() {
    document.querySelectorAll('pre').forEach((pre) => {
      const btn = document.createElement('button');
      btn.className = 'btn btn--sm';
      btn.textContent = 'Copy';
      btn.style.cssText = `
        position: absolute;
        top: 8px;
        right: 8px;
        opacity: 0;
        transition: opacity 0.2s;
      `;
      pre.style.position = 'relative';
      pre.appendChild(btn);

      pre.addEventListener('mouseenter', () => { btn.style.opacity = '1'; });
      pre.addEventListener('mouseleave', () => { btn.style.opacity = '0'; });

      btn.addEventListener('click', async () => {
        const code = pre.querySelector('code')?.textContent || pre.textContent;
        try {
          await navigator.clipboard.writeText(code);
          btn.textContent = 'Copied!';
          showToast('Code copied to clipboard', '📋');
          setTimeout(() => { btn.textContent = 'Copy'; }, 2000);
        } catch {
          btn.textContent = 'Failed';
        }
      });
    });
  }

  // ─────────────────────────────────────
  // HERO PROGRESS MINI BAR
  // ─────────────────────────────────────

  function initHeroProgressBars() {
    // Small pill progress in hero stats
    document.querySelectorAll('[data-progress-bar]').forEach((el) => {
      const pct = el.dataset.progressBar;
      el.style.width = pct + '%';
    });
  }

  // ─────────────────────────────────────
  // INIT ALL
  // ─────────────────────────────────────

  function init() {
    // Theme must be first to avoid flicker
    if (window.ThemeModule) ThemeModule.init();

    initNavbar();
    initMobileMenu();
    initSmoothScroll();
    initActiveSection();
    initReveal();
    initCounters();
    initParticles();
    initRipple();
    initBackToTop();
    initCopyCode();
    initHeroProgressBars();

    // Initialize Roadmap module
    if (window.RoadmapModule) RoadmapModule.init();

    // Initialize Search module
    if (window.SearchModule) SearchModule.init();

    console.log('%c DSA Project Roadmap ', 'background:#E3261C;color:#fff;font-weight:900;padding:4px 8px;border-radius:2px;font-family:monospace;', '| Neo-Brutalist v2 ✓');
  }

  // Run after DOM ready
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }

  // Expose utilities
  window.App = { showToast };
})();
