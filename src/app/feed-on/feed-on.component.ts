import { Component, OnInit } from '@angular/core';
import {
  FormArray,
  FormBuilder,
  FormControl,
  Validators,
} from '@angular/forms';

@Component({
  selector: 'app-feed-on',
  templateUrl: './feed-on.component.html',
  styleUrls: ['./feed-on.component.scss'],
})
export class FeedOnComponent implements OnInit {
  get useSystemTime(): boolean {
    return this.form.get('settings').get('useSystemTime').value;
  }

  get feedTimeControl(): FormArray {
    return this.form.get('feedTime') as FormArray;
  }

  readonly minutesValidator = [
    Validators.required,
    Validators.min(0),
    Validators.max(59),
  ];

  readonly hourValidator = [
    Validators.required,
    Validators.min(0),
    Validators.max(23),
  ];

  form = this.fb.group({
    feedTime: this.fb.array([this.newFeedTime()]),
    pumpTime: this.fb.group({
      onTime: ['', this.minutesValidator],
      offTime: ['', this.minutesValidator],
    }),
    aerationTime: this.fb.group({
      onTime: ['', this.hourValidator],
      offTime: ['', this.hourValidator],
    }),
    settings: this.fb.group({
      useSystemTime: [true],
      customTime: [''],
    }),
  });

  constructor(private fb: FormBuilder) {}

  ngOnInit(): void {
    console.log(this.newFeedTime());
  }

  onNewFeedTime(): void {
    this.feedTimeControl.push(this.newFeedTime());
  }

  onRemoveFeedTime(i: number): void {
    this.feedTimeControl.removeAt(i);
  }

  private newFeedTime(val?): FormControl {
    return this.fb.control(val, this.hourValidator);
  }
}
