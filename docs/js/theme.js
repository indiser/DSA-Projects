/**
 * theme.js — Dark/Light Mode Toggle
 * Neo-Brutalist Premium SaaS — DSA Project Roadmap
 *
 * Default theme is now LIGHT (Neo-Brutalist off-white).
 * Dark mode = [data-theme="dark"] attribute.
 */

(function ThemeModule() {
  'use strict';

  const STORAGE_KEY = 'dsa-theme';
  const DARK  = 'dark';
  const LIGHT = 'light';

  /**
   * Returns the preferred theme: stored > system preference > light (default)
   */
  function getPreferredTheme() {
    const stored = localStorage.getItem(STORAGE_KEY);
    if (stored === DARK || stored === LIGHT) return stored;
    if (window.matchMedia('(prefers-color-scheme: dark)').matches) return DARK;
    return LIGHT;
  }

  /**
   * Apply theme to DOM
   * @param {string} theme - 'dark' | 'light'
   */
  function applyTheme(theme) {
    const root = document.documentElement;
    const toggleBtn       = document.getElementById('theme-toggle');
    const toggleBtnMobile = document.getElementById('theme-toggle-mobile');

    if (theme === DARK) {
      root.setAttribute('data-theme', 'dark');
    } else {
      root.removeAttribute('data-theme');
    }

    const icon  = theme === DARK ? '☀️' : '🌙';
    const label = theme === DARK ? 'Switch to light mode' : 'Switch to dark mode';

    if (toggleBtn) {
      toggleBtn.textContent = icon;
      toggleBtn.setAttribute('aria-label', label);
      toggleBtn.setAttribute('title', label);
    }
    if (toggleBtnMobile) {
      toggleBtnMobile.textContent = icon;
      toggleBtnMobile.setAttribute('aria-label', label);
    }

    localStorage.setItem(STORAGE_KEY, theme);
  }

  /**
   * Toggle between dark and light
   */
  function toggle() {
    const current = localStorage.getItem(STORAGE_KEY) || LIGHT;
    const next = current === DARK ? LIGHT : DARK;
    applyTheme(next);

    // Dispatch event so other modules can react
    document.dispatchEvent(new CustomEvent('themechange', { detail: { theme: next } }));
  }

  /**
   * Initialize theme module
   */
  function init() {
    // Apply theme immediately to avoid flash
    applyTheme(getPreferredTheme());

    // Bind toggle buttons
    document.getElementById('theme-toggle')?.addEventListener('click', toggle);
    document.getElementById('theme-toggle-mobile')?.addEventListener('click', toggle);

    // Watch system preference changes
    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', (e) => {
      // Only auto-switch if user hasn't manually overridden
      if (!localStorage.getItem(STORAGE_KEY)) {
        applyTheme(e.matches ? DARK : LIGHT);
      }
    });
  }

  // Expose init globally
  window.ThemeModule = { init };
})();
