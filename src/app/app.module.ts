import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { MaterialModule } from './material.module';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FeedOnComponent } from './feed-on/feed-on.component';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { AboutDialogComponent } from './about-dialog/about-dialog.component';

@NgModule({
  declarations: [AppComponent, FeedOnComponent, AboutDialogComponent],
  imports: [
    BrowserModule,
    AppRoutingModule,
    MaterialModule,
    BrowserAnimationsModule,
    FormsModule,
    ReactiveFormsModule,
  ],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
