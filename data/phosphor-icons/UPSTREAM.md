# Phosphor icon vocabulary sources

The generated icon vocabulary combines the shipped webfont catalog with the
structured search metadata published by Phosphor Icons.

- Font catalog: `@phosphor-icons/web` 2.1.2, tag `v2.1.2`, commit
  `70854726d7bd82ae21f0dc81b5b5c35240a77066`
- Source selection manifest SHA-256:
  `4c2141ee6e8ed9b53466b26e483ca4d85fcf2044138029095d56f910e1b9dab2`
- Shipped Fill font SHA-256:
  `a53f5d2630cab5e3b7536ecb9d69d71519a2190298c22b1f8d770dd37bc2940a`
- Search metadata: `@phosphor-icons/core` 2.1.1
- Package integrity:
  `sha512-v4ARvrip4qBCImOE5rmPUylOEK4iiED9ZyKjcvzuezqMaiRASCHKcRIuvvxL/twvLpkfnEODCOJp5dM4eZilxQ==`
- Package archive SHA-256:
  `313332be6190b724da24107addd781799b48bf76b13963f24501112ffe1baadd`

`PhosphorFillCatalog.json` is the normalized name/codepoint export from the
2.1.2 Fill `selection.json`. `PhosphorMetadata.json` is the normalized
`icons` structured export from the core package. Categories are deliberately
not included in inference. `DomainIconPreferences.json` contains the small
reviewed Dear Modding semantic vocabulary.

Regenerate and verify the checked-in header offline:

```powershell
python Tools/GeneratePhosphorGlyphs.py
python Tools/GeneratePhosphorGlyphs.py --check
```

When validating against the host's bundled font, also pass:

```powershell
python Tools/GeneratePhosphorGlyphs.py --check --font <path-to-Phosphor-Fill.ttf>
```

The Phosphor inputs are licensed under the adjacent `LICENSE` file.
