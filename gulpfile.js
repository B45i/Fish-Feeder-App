const gulp = require("gulp");
const inline = require("gulp-inline");

gulp.task("default", () => {
  return gulp
    .src("./dist/fish-feeder/*.html")
    .pipe(inline())
    .pipe(gulp.dest("./feeder_arduino/data"));
});
