# Documentation for the index.json file

This file contains a technical overview of the `index.json` file, which is used to store metadata and data structure for Stellarium skycultures. The documentation is intended for developers and integrators working with Stellarium-compatible skyculture definitions.

## Parameters

### `id`
- A unique lowercase identifier string for the skyculture (e.g., `"modern"`, `"hawaiian"`).

### `region`
- Geographic or cultural region of the skyculture.
- Uses UN M49 geoscheme and Stellarium-specific tags:
```json
[
  "World", "Northern Africa", "Eastern Africa", "Central Africa", "Southern Africa", "Western Africa",
  "Caribbean", "Central America", "Northern America", "Southern America",
  "Eastern Asia", "Central Asia", "Southern Asia", "South-eastern Asia", "Western Asia", "Northern Asia",
  "Eastern Europe", "Central Europe", "Southern Europe", "Western Europe", "Northern Europe",
  "Australasia", "Melanesia", "Micronesia", "Polynesia", "Antarctica"
]
```

### `classification`
- Describes the type and origin of the skyculture:
```json
[
  "personal",       // Created for personal use or aesthetics.
  "traditional",    // Based on long-standing cultural practices (default).
  "ethnographic",   // Based on academic fieldwork/interviews.
  "historical",     // Based on historical documentation.
  "single",         // Based on a single source.
  "comparative"     // A composed culture, e.g., for visual or pedagogical comparison.
]
```

### `fallback_to_international_names`
- Type: `boolean` (`true` or `false`)
- Description: If set to `true`, Stellarium will automatically fill in missing names for stars and deep-sky objects (DSOs) using the official international naming standards.

Specifically:
- **Stars** will use IAU-approved proper names (e.g., `"Betelgeuse"` for `"HIP 27989"`).
- **DSOs (Deep Sky Objects)** such as galaxies or nebulae will use standard catalog names (e.g., `"Andromeda Galaxy"` for `"M31"`).

Use this if your skyculture doesn't provide custom names for all visible objects.


### `asterisms`
- An array of asterism objects, each with:
  - `id`: Unique string ID, e.g., `"AST hawaiian Matariki"`
  - `lines`: Array of HIP IDs or coordinate pairs.
  - `common_name`: Object with fields like `"english"` and optional `"references"`.
  - `single_star_radius` (optional): Float (e.g., `0.5`)
  - `is_ray_helper` (optional): Boolean

### `constellations`
- An array of constellation definitions:
  - `id`: E.g., `"CON modern Orion"`
  - `lines`: HIP numbers or coordinates
  - `common_name`: English and/or native label
  - `image`: Optional, with:
    - `file`, `size`, `anchors` (linking image to HIP stars)
  - `single_star_radius` (optional)

### `edges_type`
- Determines how constellation boundaries are interpreted:
```json
[
  "none",    // No boundaries
  "iau",     // Use IAU standard boundaries
  "own"      // Use culture-specific `edges` definitions
]
```

### `edges_source`
- Optional. A string with a URL or file reference for the boundary data source.

### `edges_epoch`
- Epoch reference for boundary coordinates. Allowed formats:
```json
[
  "J2000",       // Julian epoch 2000.0 (default)
  "B1875",       // Besselian epoch 1875.0 (for IAU)
  "Byyyy.y",     // Arbitrary Besselian year
  "Jyyyy.y",     // Arbitrary Julian year
  "JDddddddd.d"  // Julian Day number
]
```

### `edges`
- Array of strings describing segment boundaries in sexagesimal coordinates with identifiers. Format:
  `"001:002 M+ 22:52:00 +34:30:00 22:52:00 +52:30:00 AND LAC"`

---

### `common_names`

This dictionary maps celestial object identifiers to lists of names and metadata. Valid key types:

#### Stars
```json
"HIP 677": [
  { "english": "Alpheratz", "references": [1, 2] },
  { "english": "Sirrah", "references": [3] }
]
```

#### Planets and solar system objects
```json
"NAME Sun":    [{ "english": "Sun", "native": "Lā" }],
"NAME Moon":   [{ "english": "Moon", "native": "Māsina" }],
"NAME Mars":   [{ "english": "Reddish Face", "native": "Matamemea" }],
"NAME Venus":  [{ "english": "Morning Star", "native": "Tapu 'itea" }],
"NAME Earth":  [{ "english": "Earth", "native": "Lalolagi" }],
"NAME Jupiter":[{ "english": "Undying Mystery", "native": "Tupualēgase" }],
"NAME Saturn": [{ "english": "Garland Star", "native": "Fētu‘āsōa" }],
"NAME Mercury":[{ "english": "Brownish", "native": "Ta‘elo" }]
```

#### Deep Sky Objects (DSO)
```json
"M45":       [{ "english": "Face of Li‘i", "native": "Matāli‘i" }],
"NGC2055":   [{ "english": "Pale Cloud", "native": "Aotea" }],
"NGC292":    [{ "english": "Flying Cloud", "native": "Aolele" }],
"NGC6093":   [{ "english": "Pae" }],
"NGC6121":   [{ "english": "Suga" }]
```

---

## Notes
- The `english` field is used for display. The first entry will typically be shown in Stellarium.
- The `native` field represents the original language spelling and can include Unicode characters.
- Optional tags: `pronounce`, `references`, `translators_comments`
