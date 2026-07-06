/**
 * roadmap.js — Phase Accordion, Timeline Animations & Progress Bars
 * DSA Project Roadmap Documentation
 */

(function RoadmapModule() {
  'use strict';

  /**
   * Toggle a phase open/closed
   * @param {Element} phase
   */
  function togglePhase(phase) {
    const isOpen = phase.classList.contains('phase--open');

    if (isOpen) {
      phase.classList.remove('phase--open');
    } else {
      phase.classList.add('phase--open');
    }
  }

  /**
   * Animate progress bars when they scroll into view
   */
  function animateProgressBars() {
    const bars = document.querySelectorAll('.skill-bar[data-width]');
    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          const bar = entry.target;
          const width = bar.getAttribute('data-width');
          // Delay for staggered effect
          const delay = parseInt(bar.dataset.delay || '0', 10);
          setTimeout(() => {
            bar.style.width = width + '%';
          }, delay);
          observer.unobserve(bar);
        }
      });
    }, { threshold: 0.3 });

    bars.forEach((bar) => observer.observe(bar));
  }

  /**
   * Animate phase progress bars when in view
   */
  function animatePhaseProgressBars() {
    const bars = document.querySelectorAll('.phase__progress-mini-bar[data-width]');
    bars.forEach((bar) => {
      const width = bar.getAttribute('data-width');
      bar.style.width = width + '%';
    });

    const largeBars = document.querySelectorAll('.phase-progress-bar[data-width]');
    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          const bar = entry.target;
          bar.style.width = bar.getAttribute('data-width') + '%';
          observer.unobserve(bar);
        }
      });
    }, { threshold: 0.3 });

    largeBars.forEach((bar) => observer.observe(bar));
  }

  /**
   * Animate the circular progress indicator (SVG stroke-dashoffset)
   */
  function animateCircularProgress() {
    const circle = document.querySelector('.progress-circle-bar');
    if (!circle) return;

    const total     = 24;
    const completed = 14;
    const percent   = (completed / total);
    const circumference = 283; // 2π * r (r=45)

    const observer = new IntersectionObserver((entries) => {
      entries.forEach((entry) => {
        if (entry.isIntersecting) {
          circle.style.strokeDashoffset = circumference * (1 - percent);
          observer.unobserve(entry.target);
        }
      });
    }, { threshold: 0.5 });

    observer.observe(circle.closest('.progress-summary-card') || circle);
  }

  /**
   * Initialize phase accordion functionality
   */
  function initAccordion() {
    document.querySelectorAll('.phase__header').forEach((header) => {
      header.addEventListener('click', () => {
        const phase = header.closest('.phase');
        togglePhase(phase);
      });

      // Keyboard accessibility
      header.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          const phase = header.closest('.phase');
          togglePhase(phase);
        }
      });

      header.setAttribute('tabindex', '0');
      header.setAttribute('role', 'button');
    });

    // Open first two phases by default
    const phases = document.querySelectorAll('.phase');
    if (phases[0]) phases[0].classList.add('phase--open');
    if (phases[1]) phases[1].classList.add('phase--open');
    if (phases[2]) phases[2].classList.add('phase--open');
  }

  /**
   * Initialize
   */
  function init() {
    initAccordion();
    animateProgressBars();
    animatePhaseProgressBars();
    animateCircularProgress();
  }

  window.RoadmapModule = { init, togglePhase };
})();
