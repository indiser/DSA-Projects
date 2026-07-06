/**
 * search.js — Real-Time Project Search & Filtering
 * DSA Project Roadmap Documentation
 */

(function SearchModule() {
  'use strict';

  // ── State ──
  let activeFilters = new Set(['all']);
  let searchQuery   = '';

  /**
   * Normalize text for search comparison
   * @param {string} str
   * @returns {string}
   */
  function normalize(str) {
    return str.toLowerCase().trim();
  }

  /**
   * Get all project cards from the DOM
   * @returns {NodeListOf<Element>}
   */
  function getCards() {
    return document.querySelectorAll('.project-card');
  }

  /**
   * Determine if a card matches current search and filters
   * @param {Element} card
   * @returns {boolean}
   */
  function cardMatches(card) {
    const data = card.dataset;
    const text = normalize([
      data.title   || '',
      data.concepts|| '',
      data.tech    || '',
      data.desc    || '',
    ].join(' '));

    // Search filter
    if (searchQuery && !text.includes(normalize(searchQuery))) {
      return false;
    }

    // Status / difficulty filter
    if (!activeFilters.has('all')) {
      const status     = data.status     || '';
      const difficulty = data.difficulty || '';
      const phase      = data.phase      || '';

      let passesFilter = false;
      for (const f of activeFilters) {
        if (f === status)       { passesFilter = true; break; }
        if (f === difficulty)   { passesFilter = true; break; }
        if (f === `phase-${phase}`) { passesFilter = true; break; }
      }
      if (!passesFilter) return false;
    }

    return true;
  }

  /**
   * Re-render card visibility based on current state
   */
  function applyFilters() {
    const cards = getCards();
    let visibleCount = 0;

    cards.forEach((card) => {
      if (cardMatches(card)) {
        card.classList.remove('project-card--hidden');
        visibleCount++;
      } else {
        card.classList.add('project-card--hidden');
      }
    });

    // Show/hide phases depending on whether any children are visible
    document.querySelectorAll('.phase').forEach((phase) => {
      const phaseCards = phase.querySelectorAll('.project-card:not(.project-card--hidden)');
      const totalCards = phase.querySelectorAll('.project-card');

      if (phaseCards.length === 0 && totalCards.length > 0) {
        phase.style.display = 'none';
      } else {
        phase.style.display = '';
      }
    });

    // No results state
    const noResults = document.getElementById('no-results');
    if (noResults) {
      if (visibleCount === 0) {
        noResults.classList.add('visible');
      } else {
        noResults.classList.remove('visible');
      }
    }

    // Update visible count badge
    const countEl = document.getElementById('visible-count');
    if (countEl) {
      countEl.textContent = visibleCount;
    }
  }

  /**
   * Handle search input
   * @param {Event} e
   */
  function handleSearch(e) {
    searchQuery = normalize(e.target.value);

    // Show/hide clear button
    const clearBtn = document.getElementById('search-clear');
    if (clearBtn) {
      clearBtn.classList.toggle('visible', searchQuery.length > 0);
    }

    // Open all phases if user is searching
    if (searchQuery) {
      document.querySelectorAll('.phase').forEach((p) => p.classList.add('phase--open'));
    }

    applyFilters();
  }

  /**
   * Handle filter button clicks
   * @param {Event} e
   */
  function handleFilterClick(e) {
    const btn = e.currentTarget;
    const filter = btn.dataset.filter;

    if (filter === 'all') {
      activeFilters.clear();
      activeFilters.add('all');
    } else {
      activeFilters.delete('all');
      if (activeFilters.has(filter)) {
        activeFilters.delete(filter);
        if (activeFilters.size === 0) activeFilters.add('all');
      } else {
        activeFilters.add(filter);
      }
    }

    // Update button styles
    document.querySelectorAll('.filter-btn').forEach((b) => {
      const f = b.dataset.filter;
      b.classList.toggle('active', activeFilters.has(f));
    });

    applyFilters();
  }

  /**
   * Clear search input
   */
  function clearSearch() {
    const input = document.getElementById('search-input');
    if (input) {
      input.value = '';
      searchQuery = '';
      const clearBtn = document.getElementById('search-clear');
      if (clearBtn) clearBtn.classList.remove('visible');
      applyFilters();
    }
  }

  /**
   * Initialize search module
   */
  function init() {
    // Search input
    const searchInput = document.getElementById('search-input');
    if (searchInput) {
      searchInput.addEventListener('input', handleSearch);
      // Keyboard shortcut: Cmd/Ctrl + K
      document.addEventListener('keydown', (e) => {
        if ((e.metaKey || e.ctrlKey) && e.key === 'k') {
          e.preventDefault();
          searchInput.focus();
          searchInput.select();
        }
        // Escape to clear
        if (e.key === 'Escape' && document.activeElement === searchInput) {
          clearSearch();
          searchInput.blur();
        }
      });
    }

    // Clear button
    document.getElementById('search-clear')?.addEventListener('click', clearSearch);

    // Filter buttons
    document.querySelectorAll('.filter-btn').forEach((btn) => {
      btn.addEventListener('click', handleFilterClick);
    });

    // Set initial "All" active
    const allBtn = document.querySelector('[data-filter="all"]');
    if (allBtn) allBtn.classList.add('active');
  }

  // Expose
  window.SearchModule = { init, applyFilters };
})();
