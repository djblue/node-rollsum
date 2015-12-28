{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "./src/addon.cc",
        "./src/rollsum.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
